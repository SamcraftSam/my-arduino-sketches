#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

int sensorPin = 7;
int pumpPin = 8;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(sensorPin) == HIGH){
    Serial.println("TestResult!");
    digitalWrite(pumpPin, HIGH);
    delay(1000);
    digitalWrite(pumpPin, LOW);
    delay(1000);
  }
}
