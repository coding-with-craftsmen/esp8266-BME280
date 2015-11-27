#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);
/************************* WiFi Access Point *********************************/
#define WLAN_SSID "*******"
#define WLAN_PASS "*******"
/************************* MQTT Broker Setup *********************************/
const int MQTT_PORT = 12665;
const char MQTT_SERVER[] PROGMEM = "m11.cloudmqtt.com";
const char MQTT_CLIENTID[] PROGMEM = "ESP-PUBLISHER-SERVICE";
const char MQTT_USERNAME[] PROGMEM = "******";
const char MQTT_PASSWORD[] PROGMEM = "******";

WiFiClient client;


Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/
const char TEMPERATURE_FEED[] PROGMEM = "sweethome/sensors/outdoor/temperature";
Adafruit_MQTT_Publish temperature_topic = Adafruit_MQTT_Publish(&mqtt, TEMPERATURE_FEED);
const char PRESSURE_FEED[] PROGMEM = "sweethome/sensors/outdoor/pressure";
Adafruit_MQTT_Publish pressure_topic = Adafruit_MQTT_Publish(&mqtt, PRESSURE_FEED);

const char HUMIDITY_FEED[] PROGMEM = "sweethome/sensors/outdoor/humidity";
Adafruit_MQTT_Publish humidity_topic = Adafruit_MQTT_Publish(&mqtt, HUMIDITY_FEED);

/*************************** Sketch Code ************************************/

void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.println("Sensor Test");
  if (!bme.begin())
  {
    Serial.print("Ooops, no BME280 detected ... Check your wiring or I2C ADDR!");
    while (1);

  }
  else {
    Serial.println("BME280 ready.");
  }
  // Connect to WiFi access point.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;

  }
  Serial.print("Connecting to MQTT... ");
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    switch (ret) {
      case 1: Serial.println("Wrong protocol"); break;
      case 2: Serial.println("ID rejected"); break;
      case 3: Serial.println("Server unavailable"); break;
      case 4: Serial.println("Bad user/password"); break;
      case 5: Serial.println("Not authenticated"); break;
      case 6: Serial.println("Failed to subscribe"); break;
      default: Serial.print("Couldn't connect to server, code: ");
        Serial.println(ret);
        break;

    }
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);// wait 5 seconds

  }
  Serial.println("MQTT Connected!");
}

void loop() {

  MQTT_connect();

  
  float temperature = bme.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Publish Temperature: ");
  if (! temperature_topic.publish(temperature)) {
    Serial.println("Failed");

  } else {
    Serial.println("OK!");

  }

  float pressure = bme.readPressure() / 100.0F;
  Serial.print("Pressure:    ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Publish Pressure: ");
  if (! pressure_topic.publish(pressure)) {
    Serial.println("Failed");

  } else {
    Serial.println("OK!");

  }

  float humidity = bme.readHumidity();
  Serial.print("Humidity:    ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Publish Humidity: ");
  if (! pressure_topic.publish(humidity)) {
    Serial.println("Failed");

  } else {
    Serial.println("OK!");

  }

  delay(5000);
}



