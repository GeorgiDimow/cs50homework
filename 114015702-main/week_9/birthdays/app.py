import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        db_id = db.execute("SELECT id FROM birthdays ORDER BY id DESC LIMIT 1")[0]['id'] + 1

        name = request.form.get('Name')
        month = request.form.get('Month')
        day = request.form.get('Day')
        try:
            if(len(name) > 2 and 0 < int(month) < 13 and 0 < int(day) < 31):
                db.execute("INSERT INTO birthdays (id, name, month, day) VALUES (?, ?, ?, ?);", db_id, name, month, day)
        except ValueError:
            pass

        return redirect("/")

    else:
        db_list_of_bdays = db.execute("SELECT name, month, day FROM birthdays")

        return render_template("index.html", db_list_of_bdays=db_list_of_bdays)


if __name__ == "__main__":
    app.run()