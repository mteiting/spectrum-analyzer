#include <Arduino.h>

void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
    delay(500);
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
}