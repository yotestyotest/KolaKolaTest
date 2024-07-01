== What ==

task from: https://github.com/villsun/Salary_calculation/


Example output:

```
Welcome to KolaKola!

Main | <Parameters not set> | <Work registry empty>
[X] Exit [P] Parameters [D] Working days p
 > Parameters

Hourly wage: 0 Weekend suppl: 0 Evening suppl: 0 Tax: 0%
[A] Accept [H] $Hourly [W] $Weekend [E] $Evening [T] %Tax [D] Defaults [S] Save [L] Load l
 > Load

Hourly wage: 100 Weekend suppl: 10 Evening suppl: 11 Tax: 25%
[A] Accept [H] $Hourly [W] $Weekend [E] $Evening [T] %Tax [D] Defaults [S] Save [L] Load a
 > Accept

Main | $100/h Wknd/Evng: 10/11 Tax: 25% | <Work registry empty>
[X] Exit [P] Parameters [D] Working days d
 > Working days

<Work registry empty>
[A] Accept [I] Input new [L] Load l
 > Load
(+) 2024-02-28 7 17
(+) 2024-03-02 9 19
(+) 2024-03-03 7 17
(+) 2024-04-30 20.5 22.5

4 records, 2024-02-28 .. 2024-04-30
[A] Accept [I] Input new [C] Clear [E] Edit last [#] Edit(#) [P] Print [L] Load [S] Save a
 > Accept

Main | $100/h Wknd/Evng: 10/11 Tax: 25% | 4 records, 2024-02-28 .. 2024-04-30
[X] Exit [P] Parameters [D] Working days [C] Calculate $ c
 > Calculate $
       Total         Day       Night                      Workday
     1000.00     1000.00        0.00       2024-02-28 07:00..17:00
     1122.00      880.00      242.00       2024-03-02 09:00..19:00
     1100.00     1100.00        0.00       2024-03-03 07:00..17:00
      222.00        0.00      222.00       2024-04-30 20:30..22:30
  __________
     3444.00
     2583.00 after tax deduction (25.00%)

Main | $100/h Wknd/Evng: 10/11 Tax: 25% | 4 records, 2024-02-28 .. 2024-04-30
[X] Exit [P] Parameters [D] Working days [C] Calculate $
```
