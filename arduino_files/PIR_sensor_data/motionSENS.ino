#include <Arduino_MKRIoTCarrier.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
/* Document includes */
#include "visuals.h"
#include "arduino_secrets.h"

MKRIoTCarrier carrier;
WiFiSSLClient WiFiSSLClient;
MqttClient mqttClient(WiFiSSLClient);

const char broker[] = "1481343df7cd494380334fb5dc9a2753.s1.eu.hivemq.cloud";
int        port     = 8883;
const char topic[]  = "movement";

int motionSensor = A6;
int MSValue = 0;
int count = 0;

String json;
StaticJsonDocument<200> doc;
char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;    
int status = WL_IDLE_STATUS;


void setup() {
carrier.begin();
CARRIER_CASE = true;
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("You're connected to the network");
  Serial.println();
  mqttClient.setUsernamePassword("cbc1000", "Cbc123698745");
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
    CARRIER_CASE = false;

  pinMode(motionSensor, INPUT);
  digitalWrite(motionSensor, LOW);

  WiFiDrv::pinMode(25, OUTPUT);  //define green pin
}


void loop() {
  printData();
  Serial.println("----------------------------------------");
  detectMotion();
}

void detectMotion() {
    MSValue = digitalRead(motionSensor);
    if (MSValue == HIGH) {
    Serial.println("motion");
    WiFiDrv::analogWrite(25, 255);
    count++;
    doc["motion detectet"] = count;
    serializeJson(doc, json);
    serializeJsonPretty(doc, Serial);
    Serial.print(json);
    mqttClient.beginMessage(topic);
    mqttClient.print(json);
    mqttClient.endMessage();
    json = "";
    delay(6000);

  } else {
    Serial.println("no motion");
    WiFiDrv::analogWrite(25, 0);

    delay(250);
  }

}

void printData() {
  Serial.println("Board Info:" );
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Adresse: ");
  Serial.println(ip);
  Serial.println();
  Serial.println("Network Information");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  long rssi = WiFi.RSSI();
  Serial.print("Signal styrke");
  Serial.println(rssi);
}