-- Keep a log of any SQL queries you execute as you solve the mystery.
-- .
.tables --
.schema crime_scene_reports --
SELECT
  description
FROM
  crime_scene_reports
WHERE
  month = 7
  AND day = 28
  AND street = 'Humphrey Street';
--
.schema interviews --information about the crime
SELECT
  transcript
FROM
  interviews
WHERE
  month = 7
  AND day = 28
  AND transcript LIKE '%bakery%';
--first transcript is about the security logs
.schema bakery_security_logs --searching for the car license plate that exit
SELECT
  license_plate
FROM
  bakery_security_logs
WHERE
  month = 7
  AND day = 28
  AND hour = 10
  AND activity = 'exit';
--second transcript is about the ATM on Leggett Street
.schema atm_transactions --searching for the bank acount tha withdraw money in the same day
SELECT
  account_number
FROM
  atm_transactions
WHERE
  month = 7
  AND day = 28
  AND atm_location = 'Leggett Street'
  AND transaction_type = 'withdraw';
--third transcript is about the phone call that the thief made
.schema phone_calls --searching for all phone calls made that day that are less than one min long
SELECT
  caller,
  receiver,
  duration
FROM
  phone_calls
WHERE
  month = 7
  AND day = 28
  AND duration < 60;
--check if the date so far match any people
.schema people --finding the people that match the first 3 criteria
SELECT
  name,
  phone_number,
  license_plate,
  passport_number,
  receiver
FROM
  people
  INNER JOIN (
    SELECT
      caller,
      receiver,
      duration
    FROM
      phone_calls
    WHERE
      month = 7
      AND day = 28
      AND duration < 60
  ) ON caller = phone_number
WHERE
  license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      month = 7
      AND day = 28
      AND hour = 10
      AND activity = 'exit'
  );
--third transcript mantions that the thief booked a flight
.schema airports --finding the id of the right airport in Fiftyville
SELECT
  id,
  full_name,
  abbreviation
FROM
  airports
WHERE
  city = 'Fiftyville';
--searching for the earliest flight in the day after the crime--searching for the earliest flight in the day after the crime
.schema flights
SELECT
  id
FROM
  flights
WHERE
  origin_airport_id = (
    SELECT
      id
    FROM
      airports
    WHERE
      city = 'Fiftyville'
  )
  AND month = 7
  AND day = 29
ORDER BY
  hour,
  minute
LIMIT
  1;
--searching for the pasports of the passengers on that flight
.schema passengers
SELECT
  passport_number
FROM
  passengers
WHERE
  flight_id = (
    SELECT
      id
    FROM
      flights
    WHERE
      origin_airport_id = (
        SELECT
          id
        FROM
          airports
        WHERE
          city = 'Fiftyville'
      )
      AND month = 7
      AND day = 29
    ORDER BY
      hour,
      minute
    LIMIT
      1
  );
--mathcing the pasporsts to the first 3 critetia
SELECT
  name
FROM
  (
    SELECT
      *
    FROM
      (
        SELECT
          *
        FROM
          (
            SELECT
              *
            FROM
              people
              INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
            WHERE
              bank_accounts.account_number IN (
                SELECT
                  account_number
                FROM
                  atm_transactions
                WHERE
                  month = 7
                  AND day = 28
                  AND atm_location = 'Leggett Street'
                  AND transaction_type = 'withdraw'
              )
          )
        WHERE
          id IN (
            SELECT
              people.id
            FROM
              people
              INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
            WHERE
              month = 7
              AND day = 28
              AND hour = 10
              AND minute < 25
              AND activity = 'exit'
          )
      )
    WHERE
      phone_number IN (
        SELECT
          caller
        FROM
          phone_calls
        WHERE
          month = 7
          AND day = 28
          AND duration < 60
      )
  )
WHERE
  passport_number IN (
    SELECT
      passport_number
    FROM
      passengers
    WHERE
      flight_id = (
        SELECT
          id
        FROM
          flights
        WHERE
          origin_airport_id = (
            SELECT
              id
            FROM
              airports
            WHERE
              city = 'Fiftyville'
          )
          AND month = 7
          AND day = 29
        ORDER BY
          hour,
          minute
        LIMIT
          1
      )
  );
SELECT
  name,
  phone_number,
  license_plate,
  passport_number,
  receiver
FROM
  people
  INNER JOIN (
    SELECT
      caller,
      receiver,
      duration
    FROM
      phone_calls
    WHERE
      month = 7
      AND day = 28
      AND
      AND duration < 60
  ) ON caller = phone_number;
SELECT
  name
FROM
  people
WHERE
  phone_number IN (
    SELECT
      receiver
    FROM
      people
      INNER JOIN (
        SELECT
          caller,
          receiver,
          duration
        FROM
          phone_calls
        WHERE
          month = 7
          AND day = 28
          AND duration < 60
      ) ON caller = phone_number
    WHERE
      license_plate IN (
        SELECT
          license_plate
        FROM
          bakery_security_logs
        WHERE
          month = 7
          AND day = 28
          AND hour = 10
          AND activity = 'exit'
      )
  );
SELECT
  name,
  id
FROM
  people
  INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE
  bank_accounts.account_number IN (
    SELECT
      account_number
    FROM
      atm_transactions
    WHERE
      month = 7
      AND day = 28
      AND atm_location = 'Leggett Street'
      AND transaction_type = 'withdraw'
  );
SELECT
  name,
  id
FROM
  people
  INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
WHERE
  month = 7
  AND day = 28
  AND hour = 10
  AND minute < 25
  AND activity = 'exit';
SELECT
  name
FROM
  (
    SELECT
      name,
      people.id
    FROM
      people
      INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
    WHERE
      bank_accounts.account_number IN (
        SELECT
          account_number
        FROM
          atm_transactions
        WHERE
          month = 7
          AND day = 28
          AND atm_location = 'Leggett Street'
          AND transaction_type = 'withdraw'
      )
  )
WHERE
  id IN (
    SELECT
      people.id
    FROM
      people
      INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
    WHERE
      month = 7
      AND day = 28
      AND hour = 10
      AND minute < 25
      AND activity = 'exit'
  );
SELECT
  name
FROM
  (
    SELECT
      *
    FROM
      (
        SELECT
          *
        FROM
          people
          INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
        WHERE
          bank_accounts.account_number IN (
            SELECT
              account_number
            FROM
              atm_transactions
            WHERE
              month = 7
              AND day = 28
              AND atm_location = 'Leggett Street'
              AND transaction_type = 'withdraw'
          )
      )
    WHERE
      id IN (
        SELECT
          people.id
        FROM
          people
          INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
        WHERE
          month = 7
          AND day = 28
          AND hour = 10
          AND minute < 25
          AND activity = 'exit'
      )
  )
WHERE
  phone_number IN (
    SELECT
      caller
    FROM
      phone_calls
    WHERE
      month = 7
      AND day = 28
      AND duration < 60
  );
SELECT
  name
FROM
  people
WHERE
  phone_number IN (
    SELECT
      receiver
    FROM
      (
        SELECT
          *
        FROM
          (
            SELECT
              *
            FROM
              (
                SELECT
                  *
                FROM
                  people
                  INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
                WHERE
                  bank_accounts.account_number IN (
                    SELECT
                      account_number
                    FROM
                      atm_transactions
                    WHERE
                      month = 7
                      AND day = 28
                      AND atm_location = 'Leggett Street'
                      AND transaction_type = 'withdraw'
                  )
              )
            WHERE
              id IN (
                SELECT
                  people.id
                FROM
                  people
                  INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
                WHERE
                  month = 7
                  AND day = 28
                  AND hour = 10
                  AND minute < 25
                  AND activity = 'exit'
              )
          ) AS t1
          INNER JOIN (
            SELECT
              *
            FROM
              phone_calls
            WHERE
              month = 7
              AND day = 28
              AND duration < 60
          ) AS t2 ON t2.caller = t1.phone_number
      )
    WHERE
      passport_number IN (
        SELECT
          passport_number
        FROM
          passengers
        WHERE
          flight_id = (
            SELECT
              id
            FROM
              flights
            WHERE
              origin_airport_id = (
                SELECT
                  id
                FROM
                  airports
                WHERE
                  city = 'Fiftyville'
              )
              AND month = 7
              AND day = 29
            ORDER BY
              hour,
              minute
            LIMIT
              1
          )
      )
  );
SELECT
  *
FROM
  (
    SELECT
      *
    FROM
      (
        SELECT
          *
        FROM
          (
            SELECT
              *
            FROM
              people
              INNER JOIN bank_accounts ON bank_accounts.person_id = people.id
            WHERE
              bank_accounts.account_number IN (
                SELECT
                  account_number
                FROM
                  atm_transactions
                WHERE
                  month = 7
                  AND day = 28
                  AND atm_location = 'Leggett Street'
                  AND transaction_type = 'withdraw'
              )
          )
        WHERE
          id IN (
            SELECT
              people.id
            FROM
              people
              INNER JOIN bakery_security_logs on bakery_security_logs.license_plate = people.license_plate
            WHERE
              month = 7
              AND day = 28
              AND hour = 10
              AND minute < 25
              AND activity = 'exit'
          )
      ) AS t1
      INNER JOIN (
        SELECT
          *
        FROM
          phone_calls
        WHERE
          month = 7
          AND day = 28
          AND duration < 60
      ) AS t2 ON t2.caller = t1.phone_number
  )
WHERE
  passport_number IN (
    SELECT
      passport_number
    FROM
      passengers
    WHERE
      flight_id = (
        SELECT
          id
        FROM
          flights
        WHERE
          origin_airport_id = (
            SELECT
              id
            FROM
              airports
            WHERE
              city = 'Fiftyville'
          )
          AND month = 7
          AND day = 29
        ORDER BY
          hour,
          minute
        LIMIT
          1
      )
  );
SELECT
  city
FROM
  airports
WHERE
  id IN (
    SELECT
      destination_airport_id
    FROM
      flights
    WHERE
      origin_airport_id = (
        SELECT
          id
        FROM
          airports
        WHERE
          city = 'Fiftyville'
      )
      AND month = 7
      AND day = 29
    ORDER BY
      hour,
      minute
    LIMIT
      1
  );
