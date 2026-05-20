/*
 * ============================================
 * EMPFÄNGER - RGB-LED per Funk steuern
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
 * RGB-LED (gemeinsame Kathode):
 *   Kathode (-) → GND (über 220Ω Widerstand)
 *   Rot (Anode) → Pin 4 (über 220Ω Widerstand)
 *   Grün (Anode) → Pin 2 (über 220Ω Widerstand)
 *   Blau (Anode) → Pin 3 (über 220Ω Widerstand)
 * 
 * ============================================
 */


// ----- Funkmodul-Bibliotheken -----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Funkmodul an Pin 9 (CE) und 10 (CSN)
RF24 radio(9, 10);

// Gemeinsamer geheimer Kanal mit dem Empfänger
const byte address[6] = "00001";

// Joystick-Anschlüsse
const int VRx = A0;   // X-Achse (links/rechts)
const int VRy = A1;   // Y-Achse (oben/unten)
const int SW = 2;     // Knopf (digital)

void setup() {
  // Knopf-Pin als Eingang (interner Pull-up-Widerstand aktivieren)
  pinMode(SW, INPUT_PULLUP);

  // Seriellen Monitor starten
  Serial.begin(9600);

  // Funkmodul starten
  radio.begin();
  radio.openWritingPipe(address); // Sende-Kanal einstellen
  radio.setPALevel(RF24_PA_LOW);  // Niedrige Sendeleistung
  radio.stopListening();          // Nicht hören, sondern senden
}

void loop() {
  // Joystick-Werte lesen
  int xValue = analogRead(VRx);
  int yValue = analogRead(VRy);
  int buttonState = digitalRead(SW); // LOW = gedrückt (weil INPUT_PULLUP)

  // Prüfen, ob Funkmodul an ist
  if (!radio.isChipConnected()) {
    Serial.println("Funkmodul nicht angeschlossen!");
    delay(1000);
  }

  // BUH – hier ist das Problem:
  // Es wird JEDES MAL 'G' (Blau) gesendet, egal was der Joystick macht.
  char msg = 'G';
  radio.write(&msg, sizeof(msg));

  // Werte auf dem Serial Monitor anzeigen (nur zur Kontrolle)
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Knopf: ");
  Serial.println(buttonState == LOW ? "Gedrückt" : "Losgelassen");

  // Kurze Pause, sonst spammt es zu sehr
  delay(50);
}
