  #include <ArduinoMqttClient.h>
  #include <WiFiNINA.h>
  #include <ArduinoJson.h>
  #include <Arduino_MKRIoTCarrier.h>


#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)


WiFiSSLClient WiFiSSLClient;
MqttClient mqttClient(WiFiSSLClient);
MKRIoTCarrier carrier;

const char broker[] = "c2e79bd864f242dea67ba553e17b7e22.s2.eu.hivemq.cloud";
int        port     = 8883;
const char topic[]  = "Movement";

const long interval = 1000;
unsigned long previousMillis = 0;


int count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  StaticJsonDocument<200> doc;
    ; // wait for serial port to connect. Needed for native USB port only
  }

    carrier.noCase();
  if (!carrier.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
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

  mqttClient.setUsernamePassword("Daniel", "Daniel16!");

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

    if (carrier.Light.proximityAvailable()) {

    int proximity = carrier.Light.readProximity();
 
  mqttClient.poll();

  unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {

      previousMillis = currentMillis;
      
      StaticJsonDocument<200> doc;
      doc[" sensor "] = " Proximity ";
      JsonArray data = doc.createNestedArray(" data ");
      data.add( proximity );

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
}
