/*
  ArduinoMqttClient - WiFi Simple Sender

  This example connects to a MQTT broker and publishes a message to
  a topic once a second.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev2 board

  This example code is in the public domain.
*/

  #include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
  #include <WiFi.h>
  #include <ArduinoJson.h>
#endif

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

WiFiSSLClient WiFiSSLClient;
MqttClient mqttClient(WiFiSSLClient);

const char broker[] = "8cb67d2396f64b0496c5c7cde99a2930.s2.eu.hivemq.cloud";
int        port     = 8883;
const char topic[]  = "Daniboi og Calle kal drikker bajere";

const long interval = 1000;
unsigned long previousMillis = 0;


int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
  StaticJsonDocument<200> doc;
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
    
    
  }

  Serial.println("You're connected to the network");
  Serial.println();

  mqttClient.setUsernamePassword("Kantarellen", "Kasperersej123");
  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // to avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;
    
    StaticJsonDocument<200> doc;
    doc[" sensor "] = " gps ";
    doc[" time "] =  1351824120 ;
    JsonArray data = doc.createNestedArray(" data ");
    data.add( 48.756080 );
    data.add( 2.302038 );

    char output[256];
    serializeJson(doc, output, sizeof(output));

    Serial.println(" JSON ");
    Serial.println(output);

    mqttClient.beginMessage(topic);
    mqttClient.print(output);
    mqttClient.endMessage();

    count++;
  }
}
