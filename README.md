# esp8266-BME280
This is an arduino example project for the ESP8266 MQTT Hackathon.
The example uses the BME280 Sensor from Adafruit.

Connect pin 4 from the ESP8266 to the SDI and pin 5 from the ESP8266 to the SCK pins of the BME280 sensor. The VIN should be connected to the 3V and the ground to the ground.

The libraries needed to run this project are:

ESP8266WiFi
Adafruit_MQTT
Adafruit_MQTT_Client
Adafruit_BME280

The libraries can be imported through the Arduino IDE. Choose the Sketch/Include Library/Manage Liraries menu option to search and add the libraries.
