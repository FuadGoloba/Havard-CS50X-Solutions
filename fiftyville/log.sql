-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Checking the crime scene reports database for descriptions of the crime reports on the day theft took place
select *
from crime_scene_reports
where year = 2021
and month = 7 and
day = 28

-- Need to check the interview transcripts of the 3 witnesses present at the time
 select *
 from interviews
 where year = 2021
 and month = 7
 and day = 28

 -- 1. Check bakery security logs based on Ruth's trancript to look for license plate of cars that left the scene within 10 minutes of the crime
 select name from people
 where license_plate in
    (select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute < 25)

-- check atm_transactions for withdrawal transactions that happened on the theft day as Eugene recognised thief withdrawing at early hours of that day
 select * from atm_transactions
 where year = 2021
 and month = 7
 and day = 28
 and atm_location = 'Leggett Street'
 and transaction_type = 'withdraw'

  -- Check the bank accouunts database of the account numbers found to be making withdrawals from ATM at Leggett street to identify id of persons
 select * from bank_accounts
 where account_number in
    (select account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = 'Leggett Street' and transaction_type = 'withdraw')

-- 2. Check to get the names of people that were were making withdrawals on the theft mornnig
select name from people
where id in
(select person_id from bank_accounts where account_number in
    (select account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = 'Leggett Street' and transaction_type = 'withdraw'))

-- 3a Check who made a call on the crime day for less than 60 minutes
select name from people
where phone_number in
    (select caller from phone_calls where year = 2021 and day = 28 and month = 7 and duration < 60)

-- check id of earliest flights that left the next morning
SELECT id FROM flights
WHERE day = "29"
AND month = "7"
AND year = "2021"
ORDER BY hour,minute
LIMIT 1

-- Get passport number of passengers who booked on the early flight
select * from passengers
where flight_id in
    (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2021" ORDER BY hour,minute LIMIT 1)

-- 3b. Get names of people who bought the first flight ticket
select name from people
where passport_number in
    (select passport_number from passengers where flight_id in
        (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2021" ORDER BY hour,minute LIMIT 1))

--Meeting all conditions to find the thief
select name from people
where name in
    (select name from people where license_plate in
        (select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute >= 15 and minute < 25))
and name in
    (select name from people where id in
        (select person_id from bank_accounts where account_number in
            (select account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = 'Leggett Street' and transaction_type = 'withdraw')))
and name in
    (select name from people where phone_number in
        (select caller from phone_calls where year = 2021 and day = 28 and month = 7 and duration < 60))
and name in
    (select name from people where passport_number in
        (select passport_number from passengers where flight_id in
            (SELECT id FROM flights WHERE day = "29" AND month = "7" AND year = "2021" ORDER BY hour,minute LIMIT 1)))

-- Check for where the thief travelled
select city from airports
where id in
    (SELECT destination_airport_id FROM flights WHERE day = "29" AND month = "7" AND year = "2021" ORDER BY hour,minute LIMIT 1)

--Check who the accomplice is
select name from people
join phone_calls
on people.phone_number = phone_calls.receiver
where year = 2021
and day = 28
and month = 7
and duration < 60
and caller =
    (select phone_number from people where name = 'Bruce')