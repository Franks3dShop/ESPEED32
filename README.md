# SlotEsp32
2026 Febuary 22
Update:
  - Schematic HAL effect sensor to TLE493D-P3B6A0
  - BOM to TLE493D-P3B6A0
  - Added support for TLE493D-W2B6, the resoluttion of this sensor is way less and trigger is less precise steping about 8% during acceleration.
  - Updated code to find version A0-A3 automatically
  - Added a space character for the car name
  - Inverted the rotation of the selector

WARNING: the magnetic sensor TLE493D-P3B6 A0 is the version 3 silicon available on Digikey or mouser. Please note the older version (P2) has issues , some other guys have been able to make it working, but I was not.  Please note, the last part (A0) of the part number is related to the address, if you purchase TLE493D-P3B6 A0 it will have 0x5D, A1 it will have 0x13, and so on, please double check the part number you ordered, and the datasheet for the proper address (I think we are using the 7 bit address )

Cutting edge Slot car controller that has an ESP32 at its heart.
How to built it video:
https://www.youtube.com/watch?v=JtMKeiguHKI
![ThumbV2](https://github.com/user-attachments/assets/9b7e1479-4882-4ed7-93d0-4a9a81be73fc)


