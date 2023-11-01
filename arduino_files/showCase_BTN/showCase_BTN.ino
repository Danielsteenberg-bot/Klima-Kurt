#include <Arduino_MKRIoTCarrier.h>
#include "visuals.h"
#include "pitches.h" 

MKRIoTCarrier carrier;

uint32_t colorRed = carrier.leds.Color(200, 0, 0);
uint32_t colorGreen = carrier.leds.Color(0, 200, 0);
uint32_t colorBlue = carrier.leds.Color(0, 0, 200);
bool codeEntered = false;

int noteDurations[] = {
  4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4,
  2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8,
  4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4,
  2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 8, 8, 8, 8, 8
};
int finalMelody[] = {
  NOTE_E3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3,
  NOTE_A3, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_B3,
  NOTE_A3, NOTE_G3, NOTE_FS3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E3, NOTE_G3,
  NOTE_E3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_FS3, NOTE_E3, NOTE_G3,
  NOTE_A3, NOTE_B3, NOTE_A3, NOTE_G3, NOTE_E3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_B3,
  NOTE_A3, NOTE_G3, NOTE_FS3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E3, NOTE_G3,
};


int count = 0;
touchButtons buttons[] = {TOUCH0, TOUCH1, TOUCH2, TOUCH3, TOUCH4};
char buttonNR[] = "01234";
int xCoordinate = 85;  

void startDisplay() {
  carrier.begin();
  CARRIER_CASE = true;
  Serial.begin(9600);

  carrier.display.fillScreen(0x0000);
  carrier.display.setCursor(50, 60); 
  carrier.display.setTextSize(3); 
  carrier.display.setTextColor(0xFFFF); 
  carrier.display.print("Tast en "); 
  carrier.display.setCursor(50, 90); 
  carrier.display.print("kode"); 

  count = 0;
  xCoordinate = 85;
  for (int i = 0; i < 5; i++) {
    carrier.leds.setPixelColor(i, 0);
 }

 Serial.println("Display started.");
}

void setup() {
  startDisplay();
}

void loop() {
  if (!codeEntered) {
    btnRegister();}
  else {
    Serial.println("Waiting for 5 seconds...");
    delay(5000);
    codeEntered = false;
    Serial.println("Starting display again...");
    delay(500);
    startDisplay();
    
  }
}

void btnRegister() {
  carrier.Buttons.update();

  for (int i = 0; i < 5; i++) {
    if (carrier.Buttons.onTouchDown(buttons[i])) {
      carrier.display.setCursor(xCoordinate, 170);
      carrier.display.print(buttonNR[i]);
      carrier.leds.setPixelColor(i, colorRed);
      carrier.leds.show();

      count++;
      xCoordinate += 20;

      if (count == 5) {
        delay(1000);
        carrier.leds.fill(colorGreen, 0, 5);
        carrier.leds.show();
        playMelody();
        carrier.display.fillScreen(0x0000);
        carrier.display.setCursor(60, 30); 
        carrier.display.print("Tilykke");
        carrier.display.setCursor(60, 60); 
        carrier.display.print("Kom ind");
        delay(5000);
        carrier.leds.clear();
        carrier.leds.show();
        codeEntered = true;
      }
    }
  }

  delay(500);
}

void playMelody() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote]; // Access note duration
    carrier.Buzzer.sound(finalMelody[thisNote]); // Play the note from the melody
    delay(noteDuration);
    int pauseBetweenNotes = noteDuration * 1;
    delay(pauseBetweenNotes);
    carrier.Buzzer.noSound();
  }
}
