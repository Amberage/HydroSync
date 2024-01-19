#include <WiFi.h>

const int led = 8;
int iteracion = 0;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
  iteracion += 1;
  Serial.println("Iteración número: " + String(iteracion));
}