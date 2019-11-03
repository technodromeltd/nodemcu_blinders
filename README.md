# nodemcu-blinders
Remote control blinders based on NodeMCU

Runs small web server with interface web page to control blinders. Counts revolutions with hall sensor and small magnet attached to blinder-motor mounting part.

Features
- Control via WiFi with your phone 
- Calibrate up down pos
- All the way up/down
- 1 step up/down
- EEPROM memory used for last_pos so no need to calibrate in case of power loss 

Insert you network settings in settings.h

Materials:
- ESP8266 e.g. NodeMCU
- motor driver e.g. L298N with 5v output
- 12V motor 
- small magnet
- hall sensor e.g. oh49e
- any IKEA blinders or similar
- 3d printed mounting parts
- 12V PSU
