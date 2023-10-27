Complete List of messages from [opengarages.org](http://opengarages.org/index.php/Toyota_CAN_ID?fbclid=IwAR1qkRzXmOjWNUf7wU5yTcUsp_QzRYBQHQt779iyFUstfQD1yGXLd94Ppy8)

| CAN ID (Decimal) | CAN ID (Hex) | Data Length | Description            | Rate (seconds) | Notes                                                           |
| ---------------- | ------------ | ----------- | ---------------------- | -------------- | --------------------------------------------------------------- |
| 920              | 398          |             | Fuel                   |                |                                                                 |
| 705              | 2C1          |             | ThrottlePedal          |                |                                                                 |
| 178              | 0B2          |             | WSPD_4                 |                |                                                                 |
| 178              | 0B2          |             | WSPD_3                 |                |                                                                 |
| 176              | 0B0          |             | WSPD_2                 |                |                                                                 |
| 176              | 0B0          |             | WSPD_1                 |                |                                                                 |
| 948              | 3B4          |             | PRND                   |                |                                                                 |
| 37               | 25           |             | SteeringAngle          |                |                                                                 |
| 464              | 1D0          |             | CurrentGear            |                |                                                                 |
| 708              | 2C4          |             | Engine Speed           |                |                                                                 |
| 548              | 224          |             | Brake_Pedal            |                |                                                                 |
| 705              | 2C1          |             | ThrottlePedal2         |                |                                                                 |
| 560              | 230          |             | ParkingBrake           |                |                                                                 |
| 896              | 380          |             | AirCon                 |                |                                                                 |
| 1553             | 611          |             | Odometer               |                |                                                                 |
| 32               | 20           | 3           | Unknown                | 0.01           | Not Data change                                                 |
| 34               | 22           | 8           | Steering like signals  | 0.02           | Bytes 1-2 and 8 change data                                     |
| 35               | 23           | 7           | Unknown                | 0.02           | Bytes 1-2 and 7 change                                          |
| 37               | 25           | 8           | Steering Data          | 0.01           | Signed data                                                     |
| 176              | 0B0          | 6           | Wheel Based Speed 1 &2 | 0.01           | Bytes 1 and 2 give a wheel speed                                |
| 176              | 0B0          | 6           | Wheel Based Speed 1 &2 | 0.01           | Bytes 3 and 4 give a wheel speed                                |
| 178              | 0B2          | 6           | Wheel Based Speed 1 &2 | 0.01           | Bytes 1 and 2 give a wheel speed                                |
| 178              | 0B2          | 6           | Wheel Based Speed 1 &2 | 0.01           | Bytes 3 and 4 give a wheel speed                                |
| 547              | 223          | 8           | Unknown                | 0.03           | Little data changes                                             |
| 548              | 224          | 8           | Brake Pressure         | 0.03           | Bytes 5 and 6                                                   |
| 705              | 2C1          | 8           | Throttle Pedal         | 0.99           | Byte 7 is throttle                                              |
| 708              | 2C4          | 8           | Engine Speed           | 0.02           | Bytes 1 and 2                                                   |
| 720              | 2D0          | 8           | Unknown                | 0.03           | No time trends                                                  |
| 896              | 380          | 8           | Unknown (Air Cond)     | 1              | Little data changes                                             |
| 897              | 381          | 8           | Unknown                | 5              | Little data changes                                             |
| 899              | 383          | 8           | Unknown                | 2              | All zeros                                                       |
| 906              | 38A          | 5           | Unknown                | 2              | All zeros                                                       |
| 920              | 398          | 2           | Fuel                   | Aperiodic      |                                                                 |
| 921              | 399          | 5           | Unknown                | 1              | Switch like data                                                |
| 928              | 3A0          | 8           | Shifting?              | 0.1            |                                                                 |
| 929              | 3A1          | 8           |                        | 1              | No changing data                                                |
| 944              | 3B0          | 6           |                        | 1              | No changing data                                                |
| 945              | 3B1          | 8           |                        | 1              | No changing data                                                |
| 947              | 3B3          | 3           | Throttle 2             | 0.5            |                                                                 |
| 948              | 3B4          | 8           | Unknown                | Aperiodic      |                                                                 |
| 951              | 3B7          | 8           | PRND                   | Aperiodic      | All zeros (Car was in drive the whole time)                     |
| 1059             | 423          | 1           |                        | 1 sec          | All zeros                                                       |
| 1088             | 440          | 8           |                        | 0.2            | All zeros                                                       |
| 1090             | 442          | 8           |                        | 0.2            | No changes                                                      |
| 1217             | 4C1          | 8           |                        | 1              | No changes                                                      |
| 1219             | 4C3          | 8           |                        | 1              | No changes                                                      |
| 1222             | 4C6          | 8           |                        | 1              | No changes                                                      |
| 1223             | 4C7          | 8           |                        | 1              | No changes                                                      |
| 1244             | 4DC          | 8           |                        | 1              | No changes                                                      |
| 1245             | 4DD          | 8           |                        | 1              | No changes                                                      |
| 1552             | 610          | 8           | Vehicle speed          | 0.5            | Byte 3 is vehicle speed (raw kph). Static bits on bytes 4 and 5 |
| 1553             | 611          | 8           |                        | 1              | Switch bit on Byte 2 only. All others static                    |
| 1568             | 620          | 8           | Switch data            | 0.3            | Switched while moving. Byte 4 changes from 55 to 56.            |
| 1569             | 621          | 8           |                        | 1              | No changing data (mostly zeros)                                 |
| 1570             | 622          | 8           |                        | 1              | No changes                                                      |
| 1572             | 624          | 8           |                        | 1              | No changes                                                      |
| 1584             | 630          | 8           |                        | 1              | All zeros                                                       |
| 1592             | 638          | 8           |                        | 1              | All zeros                                                       |

## List of messages that should be implemented (TBD)

| CAN ID (Hex) | DLC | Description                     | Rate (seconds) | Notes                                                           |
| ------------ | --- | ------------------------------- | -------------- | --------------------------------------------------------------- |
| 2C4          | 8   | Engine Speed                    | 0.02           | Bytes 1 and 2                                                   |
| 610          | 8   | Vehicle speed                   | 0.5            | Byte 3 is vehicle speed (raw kph). Static bits on bytes 4 and 5 |
| 0B0          | 6   | WSPD_1\nWheel Based Speed 1 & 2 | 0.01           | Bytes 1 and 2 give a wheel speed                                |
| 0B0          | 6   | WSPD_2\nWheel Based Speed 1 & 2 | 0.01           | Bytes 3 and 4 give a wheel speed                                |
| 0B2          | 6   | WSPD_3\nWheel Based Speed 1 & 2 | 0.01           | Bytes 1 and 2 give a wheel speed                                |
| 0B2          | 6   | WSPD_4\nWheel Based Speed 1 & 2 | 0.01           | Bytes 3 and 4 give a wheel speed                                |
