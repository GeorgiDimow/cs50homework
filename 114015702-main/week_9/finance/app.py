import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    list_of_stocks = db.execute("SELECT symbol, shares, price, shares * price as total FROM stocks WHERE user_id = ? AND shares > 0;", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)[0]["cash"]

    total = sum([entry["price"] * entry["shares"] for entry in list_of_stocks])

    for stock in list_of_stocks:
        stock["price"] = usd(stock["price"])
        stock["total"] = usd(stock["total"])

    return render_template("index.html", db_list_of_stocks=list_of_stocks, cash=usd(cash), total=usd(total + cash))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Must enter symbol")

        if not shares:
            return apology("Must enter shares")

        lookup_response = lookup(request.form.get("symbol"))

        shares = int(shares)

        if lookup_response:
            symbol = symbol.upper()
            stock_price = lookup_response["price"]
            user_id = session["user_id"]
            stock = db.execute("SELECT * FROM stocks WHERE symbol = ? AND user_id = ?;", symbol, user_id)
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

            if shares * stock_price > user_cash:
                    return apology("Can't aford")

            if not stock:
                db.execute("INSERT INTO stocks (symbol, shares, price, user_id) VALUES (?, ?, ?, ?);", symbol, 0, stock_price, user_id)
                stock = db.execute("SELECT id FROM stocks WHERE symbol = ? AND user_id = ?;", symbol, user_id)

            db.execute("INSERT INTO transactions (time, shares_traded, stock_id) VALUES (DATETIME('now'), ?, ?);", shares, stock[0]["id"])

            db.execute("UPDATE stocks SET shares = shares + ? WHERE symbol = ? AND user_id = ?;", shares, symbol, user_id)

            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?;", stock_price * shares, user_id)

        else:
            return apology("Invalid symbol")

        return redirect("/")

    elif request.method == "GET":
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    user_id = session["user_id"]

    list_of_stocks = db.execute("SELECT symbol, shares_traded, price ,time FROM transactions LEFT JOIN stocks ON transactions.stock_id = stocks.id WHERE stocks.user_id = ?;", user_id)

    for stock in list_of_stocks:
        stock["price"] = usd(stock["price"])

    return render_template("history.html", db_list_of_transactions=list_of_stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        lookup_response = lookup(request.form.get("symbol"))

        if not lookup_response:
            return apology("Invalid quote", code=403)

        return render_template("quoted.html", symbol=lookup_response['symbol'], price=lookup_response['price'])

    elif request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    username = request.form.get("username")
    password = request.form.get("password")
    confirm_pass = request.form.get("confirmation")


    if request.method == "POST":
        if not username:
            return apology("must provide username", 403)
        elif not password:
            return apology("must provide password", 403)
        elif not password == confirm_pass :
            return apology("passwords must match", 403)

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) == 1:
            return apology("Username is taken", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
        symbols = [row["symbol"] for row in db.execute("SELECT symbol FROM stocks WHERE user_id = ? AND shares > 0;", session["user_id"])]

        if request.method == "POST":

            symbol = request.form.get("symbol")
            shares = request.form.get("shares")

            if not symbol:
                return apology("Must enter symbol")

            if not shares:
                return apology("Must enter shares")

            lookup_response = lookup(request.form.get("symbol"))

            shares = int(shares)

            if lookup_response:

                stock_price = lookup_response["price"]
                user_id = session["user_id"]
                stock = db.execute("SELECT * FROM stocks WHERE symbol = ? AND user_id = ?;", symbol, user_id)

                if not stock:
                    return apology("This user does not have this stock")

                if shares > stock[0]["shares"]:
                    return apology("Too many shares")

                db.execute("INSERT INTO transactions (time, shares_traded, stock_id) VALUES (DATETIME('now'), ?, ?);", -shares, stock[0]["id"])

                db.execute("UPDATE stocks SET shares = shares - ? WHERE symbol = ? AND user_id = ?;", shares, symbol, user_id)

                db.execute("UPDATE users SET cash = cash + ? WHERE id = ?;", stock_price * shares, user_id)

            else:
                return apology("Invalid symbol")

            return redirect("/")

        elif request.method == "GET":
            return render_template("sell.html", db_list_of_symbols=symbols)


if __name__ == "__main__":
    app.run()