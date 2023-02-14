-- Logging Steps --

-- crime scene reports --
SELECT description FROM crime_scene_reports
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND street = "Humphrey Street";
-- crime occurred @ 10:15 am, 3 interviews with witnesses present, all mention bakery --
-- next step is to consider their interviews --

-- gather interview info --
SELECT name, transcript FROM interviews
    WHERE year = 2021
    AND month = 7
    AND day = 28;
-- RUTH: thief was spotted leaving the bakery parking lot within 10 min of the theft --
-- EUGENE: thief was spotted @ ATM on Leggett street earlier in the morning withdrawing money; someone second witness recognizes --
-- RAYMOND: thief was spotted on a < 1 min call w/ someone after leaving the bakery; they are leaving on the earliest flight tomorrow purchased by the receiver of the call --

-- lead 1: bakery security --
SELECT activity, license_plate FROM bakery_security_logs
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute >= 15
        AND minute <= 25;
-- 8 potential license plates --

-- lead 2: ATM --
SELECT account_number FROM atm_transactions
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND atm_location = "Leggett Street"
        AND transaction_type = "withdraw";

SELECT DISTINCT bank_accounts.person_id FROM bank_accounts
    JOIN atm_transactions
        ON bank_accounts.account_number IN (SELECT account_number FROM atm_transactions
                                                WHERE year = 2021
                                                    AND month = 7
                                                    AND day = 28
                                                    AND atm_location = "Leggett Street"
                                                    AND transaction_type = "withdraw");
-- 8 potential account numbers/people --

-- lead 3: phone call --
SELECT caller, receiver FROM phone_calls
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND duration < 60;
-- 9 potential calls --

-- lead 3: earliest flight --
SELECT * FROM airports;

SELECT id, destination_airport_id, hour, minute FROM flights
    WHERE year = 2021
        AND month = 7
        AND day = 29
        and origin_airport_id = 8
    ORDER BY hour ASC
    LIMIT 1;
-- Flight ID: 36, Destination_ID: 4 (NYC), leaves @ 8:20 am --

-- lead 3: passengers --
SELECT passport_number, seat FROM passengers
    WHERE flight_id = 36;
-- 8 potential passengers --

-- match all leads info for thief --
SELECT DISTINCT people.name FROM people
    JOIN bakery_security_logs
        ON people.license_plate = bakery_security_logs.license_plate
    JOIN bank_accounts
        ON people.id = bank_accounts.person_id
    JOIN phone_calls
        ON people.phone_number = phone_calls.caller
    JOIN passengers
        ON people.passport_number = passengers.passport_number
    WHERE people.license_plate IN (SELECT license_plate FROM bakery_security_logs
                                        WHERE year = 2021
                                            AND month = 7
                                            AND day = 28
                                            AND hour = 10
                                            AND minute >= 15
                                            AND minute <= 25)
        AND people.id IN (SELECT DISTINCT bank_accounts.person_id FROM bank_accounts
                            JOIN atm_transactions
                                ON bank_accounts.account_number
                                IN (SELECT account_number FROM atm_transactions
                                        WHERE year = 2021
                                            AND month = 7
                                            AND day = 28
                                            AND atm_location = "Leggett Street"
                                            AND transaction_type = "withdraw"))
        AND people.phone_number IN (SELECT caller FROM phone_calls
                                        WHERE year = 2021
                                            AND month = 7
                                            AND day = 28
                                            AND duration < 60)
        AND people.passport_number IN (SELECT passport_number FROM passengers
                                            WHERE flight_id = 36);
-- BRUCE is the thief --

-- match thief to accomplice --
SELECT DISTINCT people.name FROM people
    JOIN phone_calls
        ON people.phone_number = phone_calls.receiver
    WHERE people.phone_number
        IN (SELECT receiver FROM phone_calls
                JOIN people
                    ON phone_calls.caller = people.phone_number
                WHERE people.name LIKE "Bruce"
                    AND year = 2021
                    AND month = 7
                    AND day = 28
                    AND duration < 60);