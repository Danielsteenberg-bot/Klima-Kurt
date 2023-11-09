/* Libray includes */
#include <Arduino_MKRIoTCarrier.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>

/* Document includes */
#include "visuals.h"
#include "pitches.h" 
#include "arduino_secrets.h"

/* Init */
MKRIoTCarrier carrier;
WiFiSSLClient WiFiSSLClient;
MqttClient mqttClient(WiFiSSLClient);

/* Farver variabler */
uint32_t colorRed = carrier.leds.Color(200, 0, 0);
uint32_t colorGreen = carrier.leds.Color(0, 200, 0);
uint32_t colorBlue = carrier.leds.Color(0, 0, 200);


/* MQTT  VARIABLER */
const char broker[] = "c2e79bd864f242dea67ba553e17b7e22.s2.eu.hivemq.cloud";
int        port     = 8883;
const char topic[]  = "Kim";

/* Tidsvariabler til at sende data */
const long interval = 1000;
unsigned long previousMillis = 0;

/* Json variable */
String json;
StaticJsonDocument<200> doc;



/* Melodi variable */
int finalMelody[] = {
  NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
  NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16,

  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
  NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16,
  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
  
  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  NOTE_C5,4, NOTE_A4,-8, NOTE_C5,16, NOTE_E5,2,

  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7 
  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,

  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
};

/* Variabler til at holde inputs */
int count = 0;
touchButtons buttons[] = {TOUCH0, TOUCH1, TOUCH2, TOUCH3, TOUCH4};
char buttonNR[] = "01234";
int xCoordinate = 85;
String code = "";  

/* Variabler til at læse fra secrets.h */
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

  defaultDisplay();

}

void loop() {
 delay(1000);
 printData();
 Serial.println("----------------------------------------");
 btnRegister();
}

void defaultDisplay() {
  carrier.display.fillScreen(0x0000);
  carrier.display.setCursor(50, 60); 
  carrier.display.setTextSize(3); 
  carrier.display.setTextColor(0xFFFF); 
  carrier.display.print("Tast en "); 
  carrier.display.setCursor(50, 90); 
  carrier.display.print("kode"); 
}




void btnRegister() {
  carrier.Buttons.update();

  for (int i = 0; i < 5; i++) {
    if (carrier.Buttons.onTouchDown(buttons[i])) {
      carrier.display.setCursor(xCoordinate, 170);
      carrier.display.print(buttonNR[i]);
      code += String(buttonNR[i]);
      carrier.leds.setPixelColor(i, colorRed);
      carrier.leds.show();
      count++;
      xCoordinate += 20;

      if (count == 5) {
        carrier.leds.fill(colorGreen, 0, 5);
        carrier.leds.show();
        playMelody();
        carrier.display.fillScreen(0x0000);
        carrier.display.setCursor(60, 30); 
        carrier.display.print("Tilykke");
        carrier.display.setCursor(60, 60); 
        carrier.display.print("Kom ind");
        carrier.leds.clear();
        carrier.leds.show();
        count = 0;
        xCoordinate = 85;

        doc["code"] = code;
        serializeJson(doc, json);
        serializeJsonPretty(doc, Serial);
        Serial.print(json);

        mqttClient.beginMessage(topic);
        mqttClient.print(json);
        mqttClient.endMessage();

        code = "";
        defaultDisplay();
      }
    }
  }

  delay(500);
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


void playMelody() {
  int thisNote = 0;
  int noteDuration = 0;
  int pauseBetweenNotes = 0;
  
  int notes = sizeof(finalMelody) / sizeof(finalMelody[0]) / 2;

  int tempo = 10800;  

  while (thisNote < notes * 2) {
    int divider = finalMelody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (60000 * 4) / tempo / divider;
    } else if (divider < 0) {
      noteDuration = (60000 * 4) / tempo / abs(divider);
      noteDuration *= 1.5;
    }

    carrier.Buzzer.sound(finalMelody[thisNote]); 
    delay(noteDuration * 0.9); 
    carrier.Buzzer.noSound(); 

    pauseBetweenNotes = noteDuration - (noteDuration * 0.9); 
    delay(pauseBetweenNotes);

    thisNote += 2; 
  }
}
