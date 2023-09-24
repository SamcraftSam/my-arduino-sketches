
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
int angleV[3];

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo myServo;
Servo myServo2;



void setup() {
 
  Serial.begin(9600); //открываем порт для связи с ПК
  pinMode(5, OUTPUT);
  myServo.attach(8);
  myServo2.attach(7);
  radio.begin();
  radio.setAutoAck(false);      // настройка nrf
  radio.setRetries(0, 15);  
  radio.setPayloadSize(32); 
  radio.setChannel(0x60);
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate (RF24_250KBPS); 

  radio.powerUp();
  radio.startListening();
}

void loop() {
  //delay(5);
  if ( radio.available()) {
    //int angleV = 50;
    radio.read(&angleV, sizeof(angleV));
    myServo.write(angleV[0]);
    myServo2.write(angleV[1]);
    Serial.println("CONECTED");
    /*if (angleV[3] == HIGH){
      digitalWrite(5, HIGH); 
    Serial.println("MOTOR");
    } else if (angleV[3] == LOW) {
      digitalWrite(5, LOW);
      Serial.println(angleV[3]);
    }*/
    digitalWrite(5, angleV[2]);
    Serial.println(angleV[3]);
    Serial.println(angleV[2]);
    delay(10);
  } else {
    digitalWrite(5, HIGH);
    Serial.println("FAIL");
  }
}
