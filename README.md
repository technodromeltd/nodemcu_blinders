# nodemcu-blinders
Remote control blinders based on NodeMCU

Runs small web server with interface web page to control blinders. Counts revolutions with hall sensor and small magnet attached to blinder-motor mounting part.

![img](https://github.com/technodromeltd/nodemcu_blinders/blob/master/app_interface.jpg)


Features
- Control via WiFi 
- Works with any device as a web app
- Calibrate up/down pos
- All the way up/down
- 1 step up/down
- EEPROM memory used for last_pos so no need to calibrate in case of power loss 
- OTA updates


General Instructions:
1. Print 3D parts
2. Assemble and connect esp8266, motor, sensor, pse
3. Insert you network settings in settings.h.example and remove .example and upload sketch 
4. Remove Ikea blinder's spring mechanism and insert adapter instead
5. Connect to device ip addr, save as bookmark and put to homescreen
6. You can now open app in fullscreen as any native apps from your phone's homescreen

Materials:
- ESP8266 e.g. NodeMCU
- motor driver e.g. L298N with 5v output
- 12V motor 
- small magnet
- hall sensor e.g. oh49e
- any IKEA blinders or similar
- 3d printed mounting parts
- 12V PSU
