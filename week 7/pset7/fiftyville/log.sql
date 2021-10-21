-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Show all tables
.tables

-- show the CREATE TABLE command used to create crime_scene_reports
.schema crime_scene_reports

-- show description of a crime_scene_reports at month = 7 AND day = 28 AND street="Chamberlin Street"
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28
AND street="Chamberlin Street";

-- output:
/*
description
Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted
today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.
*/

-- Crime data:
-- Date: Jule 28
-- Location: Chamberlin Street
-- Time: 10:15 am

-- show the CREATE TABLE command used to create interviews
.schema interviews

-- show name, transcript from interviews at month=7 AND day = 28
SELECT name, transcript
FROM interviews
WHERE month=7 AND day = 28;

-- Output: (removed non-important data)
/*
name    |   transcript
Ruth    |   Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and
            drive away. If you have security footage from the courthouse parking lot, you might want to look for cars
            that left the parking lot in that time frame.
Eugene  |   I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at
            the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
Raymond |   As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In
            the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville
            tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/

-- notes
/*
clue1 found in line 40: "left the parking lot in that time frame"
clue2 found in line 42: "I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money (morning)"
clue3 found in line 44: "talked to them for less than a minute"
clue4 found in line 44: "they were planning to take the earliest flight out of Fiftyville tomorrow"
*/

----------------------------------------------------------------------------------------
-- Gathering Data
----------------------------------------------------------------------------------------

-- show the CREATE TABLE command used to create courthouse_security_logs
.schema courthouse_security_logs

-- show activity, license_plate, minute at month=7 AND day = 28 AND hour = 10 in the courthouse_security_logs
SELECT activity, license_plate
FROM courthouse_security_logs
WHERE month=7 AND day = 28 AND hour = 10;

-- Output: (removed non-important data based on the clues) (clue1 data)
/*
activity | license_plate    | minute
exit     | 5P2BI95          | 16
exit     | 94KL13X          | 18
exit     | 6P58WS2          | 18
exit     | 4328GD8          | 19
exit     | G412CB7          | 20
exit     | L93JTIZ          | 21
exit     | 322W7JE          | 23
exit     | 0NTHK55          | 23
exit     | 1106N58          | 35
*/

-- show the CREATE TABLE command used to create atm_transactions
.schema atm_transactions

-- show account_number, amount from atm_transactions at month=7 AND day=28 AND atm_location="Fifer Street"
-- AND transaction_type="withdraw"
SELECT account_number, amount
FROM atm_transactions
WHERE month=7 AND day=28 AND transaction_type="withdraw" AND atm_location="Fifer Street";

-- Output: (clue2 data)
/*
account_number  | amount
28500762        | 48
28296815        | 20
76054385        | 60
49610011        | 50
16153065        | 80
25506511        | 20
81061156        | 30
26013199        | 35
*/

-- show the CREATE TABLE command used to create phone_calls
.schema phone_calls

-- show caller and receiver number from phone_calls at month=7 AND day=28 AND duration<60
SELECT caller, receiver
FROM phone_calls
WHERE month=7 AND day=28 AND duration<60;

-- Output: (clue3 data)
/*
caller         | receiver
(130) 555-0289 | (996) 555-8899
(499) 555-9472 | (892) 555-8872
(367) 555-5533 | (375) 555-8161
(499) 555-9472 | (717) 555-1342
(286) 555-6063 | (676) 555-6554
(770) 555-1861 | (725) 555-3243
(031) 555-6622 | (910) 555-3251
(826) 555-1652 | (066) 555-9701
(338) 555-6650 | (704) 555-2131
*/

-- show the CREATE TABLE command used to create flights
.schema flights

-- show origin_airport_id, destination_airport_id from flights at month=7 AND day=28 and order them by hour, minute
SELECT id, origin_airport_id, destination_airport_id
FROM flights
WHERE month=7 AND day=29
ORDER BY hour, minute;

-- Output: (clue4 data)
/*
id | origin_airport_id  | destination_airport_id
36 | 8                  | 4
43 | 8                  | 1
23 | 8                  | 11
53 | 8                  | 9
18 | 8                  | 6
*/

----------------------------------------------------------------------------------------
-- Processing Data to find the thief, destiny, ACCOMPLICE
----------------------------------------------------------------------------------------

-- processing clue1 data to find the name of each person using his license_plate
SELECT name, courthouse_security_logs.license_plate
FROM courthouse_security_logs
JOIN people ON people.license_plate=courthouse_security_logs.license_plate
WHERE month=7 AND day = 28 AND hour = 10 AND activity="exit" AND minute >= "15" AND minute < "25";

-- Output:
/*
name        | license_plate
Patrick     | 5P2BI95
Ernest      | 94KL13X
Amber       | 6P58WS2
Danielle    | 4328GD8
Roger       | G412CB7
Elizabeth   | L93JTIZ
Russell     | 322W7JE
Evelyn      | 0NTHK55
*/


-- processing clue2 data to find the name of each person using his account_number
SELECT name, atm_transactions.account_number, amount
FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number=bank_accounts.account_number
JOIN people ON people.id=bank_accounts.person_id
WHERE month=7 AND day=28 AND transaction_type="withdraw" AND atm_location="Fifer Street";

-- Output:
/*
name        | account_number    | amount
Ernest      | 49610011          | 50
Russell     | 26013199          | 35
Roy         | 16153065          | 80
Bobby       | 28296815          | 20
Elizabeth   | 25506511          | 20
Danielle    | 28500762          | 48
Madison     | 76054385          | 60
Victoria    | 81061156          | 30
*/

-- processing clue3 data to find the name of each caller/receiver using his phone_number
SELECT c.name, caller, r.name, receiver
FROM phone_calls
JOIN people AS c ON c.phone_number=phone_calls.caller
JOIN people AS r ON r.phone_number=phone_calls.receiver
WHERE month=7 AND day=28 AND duration<60;

-- Output:
/*
name        | caller            | name          | receiver
Roger       | (130) 555-0289    | Jack          | (996) 555-8899
Evelyn      | (499) 555-9472    | Larry         | (892) 555-8872
Ernest      | (367) 555-5533    | Berthold      | (375) 555-8161
Evelyn      | (499) 555-9472    | Melissa       | (717) 555-1342
Madison     | (286) 555-6063    | James         | (676) 555-6554
Russell     | (770) 555-1861    | Philip        | (725) 555-3243
Kimberly    | (031) 555-6622    | Jacqueline    | (910) 555-3251
Bobby       | (826) 555-1652    | Doris         | (066) 555-9701
Victoria    | (338) 555-6650    | Anna          | (704) 555-2131
*/

-- processing clue4 data to find the name of each passenger using his flight_id
SELECT name, flights.id
FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id = (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2020"
        ORDER BY hour, minute
        LIMIT 1);
-- Output:
/*
name        | id
Doris       | 36
Roger       | 36
Ernest      | 36
Edward      | 36
Evelyn      | 36
Madison     | 36
Bobby       | 36
Danielle    | 36
*/

-- Identifying the theif by taking the intersection of the above queries (processed data)
SELECT name
FROM courthouse_security_logs
JOIN people ON people.license_plate=courthouse_security_logs.license_plate
WHERE month=7 AND day = 28 AND hour = 10 AND activity="exit" AND minute >= "15" AND minute < "25"
INTERSECT
SELECT name
FROM atm_transactions
JOIN bank_accounts ON atm_transactions.account_number=bank_accounts.account_number
JOIN people ON people.id=bank_accounts.person_id
WHERE month=7 AND day=28 AND transaction_type="withdraw" AND atm_location="Fifer Street"
INTERSECT
SELECT c.name
FROM phone_calls
JOIN people AS c ON c.phone_number=phone_calls.caller
JOIN people AS r ON r.phone_number=phone_calls.receiver
WHERE month=7 AND day=28 AND duration<60
INTERSECT
SELECT name
FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
WHERE flight_id = (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2020"
        ORDER BY hour, minute
        LIMIT 1);

-- Output: (the theif)
/*
name
Ernest
*/

-- Get the destiny
SELECT city
FROM airports
WHERE id = (SELECT destination_airport_id FROM flights WHERE day = "29" AND month = "7"
            ORDER BY hour, minute
            LIMIT 1);

-- Output: (the density)
/*
city
London
*/

-- Get the ACCOMPLICE
SELECT name
FROM people
JOIN phone_calls ON people.phone_number=phone_calls.receiver
WHERE day = "28" AND month = "7" AND duration < "60" AND caller = (
        SELECT phone_number
        FROM people
        WHERE name = "Ernest");

-- Output: (ACCOMPLICE)
/*
name
Berthold
*/