#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(9, 10); // CE, CSN
 
const byte address[6] = "00001";

const int VRx = A0;
const int VRy = A1;
const int SW = 2;      // Joystick-Button


void setup() {
 pinMode(SW, INPUT_PULLUP);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}
 
void loop() {
  int xValue = analogRead(VRx);
  int yValue = analogRead(VRy);
  int buttonState = digitalRead(SW);

  if (!radio.isChipConnected()){
    Serial.println("radio not connected");
   delay(1000);
  }
 char msg = 'G';
 radio.write(&msg, sizeof(msg));
 
 Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Button: ");
  Serial.println(buttonState == LOW ? "Pressed" : "Released");
  
}