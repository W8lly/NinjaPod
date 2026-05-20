

// Pin-Zuweisung
const int RED_PIN = 4;
const int GREEN_PIN = 2;
const int BLUE_PIN = 3;
 
// Aktueller Farbwert (0–255)
int red = 0;
int green = 0;
int blue = 0;
 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
 
void setup() {
   pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
}
 
void loop() {
  if (!radio.isChipConnected()){
    Serial.println("radio not connected");
    delay(1000);
  }
  if (radio.available()) {
    char received;
    radio.read(&received, sizeof(received));
    Serial.print("Empfangen: ");
    Serial.println(received);
 
    if (received == 'F') {
      setColor(255, 0, 0);  // LED an
    } else if (received == 'T') {
      setColor(0, 255, 0);   // LED aus
    }
    else if (received == 'G') {
      setColor(0, 0, 255);
    }
  }
  //else{
    //Serial.println("no radio available");
    //delay(1000);
  //}
}
void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}