/*
 * ============================================
 * SENDER - Joystick per Funk an Empfänger senden
 * ============================================
 * 
 * VERKABELUNG:
 * ------------
 * nRF24L01 Funkmodul:
 *   VCC  → 3.3V  (ACHTUNG: NICHT 5V!)
 *   GND  → GND
 *   CE   → Pin 9
 *   CSN  → Pin 10
 *   SCK  → Pin 13
 *   MOSI → Pin 11
 *   MISO → Pin 12
 * 
 * Joystick-Modul:
 *   GND  → GND
 *   +5V  → 5V
 *   VRx  → A0  (X-Achse, links/rechts)
 *   VRy  → A1  (Y-Achse, oben/unten)
 *   SW   → Pin 2 (Knopf, mit INPUT_PULLUP)
 * 
 * ============================================
 */


// ----- Welche Pins für die RGB-LED? -----
const int RED_PIN = 4;   // Roter Licht-Pin
const int GREEN_PIN = 2; // Grüner Licht-Pin
const int BLUE_PIN = 3;  // Blauer Licht-Pin

// ----- Aktuelle Farbwerte (später vielleicht mal nützlich) -----
int red = 0;
int green = 0;
int blue = 0;

// ----- Funkmodul-Bibliotheken -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ----- Funkmodul anschließen: CE an Pin 9, CSN an Pin 10 -----
RF24 radio(9, 10);

// ----- Gemeinsame Funk-Adresse (wie ein geheimer Kanal) -----
const byte address[6] = "00001";

void setup() {
  // LED-Pins als Ausgang schalten
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Seriellen Monitor starten (zum Debuggen)
  Serial.begin(9600);

  // Funkmodul starten
  radio.begin();
  radio.openReadingPipe(0, address); // Empfangs-Kanal einstellen
  radio.setPALevel(RF24_PA_LOW);     // Sendeleistung niedrig (reicht für kurze Distanz)
  radio.startListening();            // Auf Empfang stellen (Hören)
}

void loop() {
  // Prüfen, ob Funkmodul überhaupt an ist
  if (!radio.isChipConnected()) {
    Serial.println("Funkmodul nicht angeschlossen!");
    delay(1000);
  }

  // Wenn Daten ankommen ...
  if (radio.available()) {
    char received;                         // Platz für empfangenes Zeichen
    radio.read(&received, sizeof(received)); // Zeichen lesen
    Serial.print("Empfangen: ");
    Serial.println(received);

    // Je nach Buchstabe: andere Farbe einschalten
    if (received == 'F') {
      setColor(255, 0, 0);  // Rot
    } else if (received == 'T') {
      setColor(0, 255, 0);  // Grün
    } else if (received == 'G') {
      setColor(0, 0, 255);  // Blau
    }
  }
}

// ----- Eigene Funktion: LED-Farbe setzen (Rot, Grün, Blau jeweils 0-255) -----
void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}
