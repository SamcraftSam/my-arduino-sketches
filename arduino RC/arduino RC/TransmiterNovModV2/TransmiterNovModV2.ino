
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define BUZZ 3
RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

int potValue[3];

int ch1_value;              // переменные для каналов
int ch2_value;

void setup() {
  delay(10);
  Serial.begin(9600);

  radio.begin();           // запуск модуля nrf
  delay(1000);

  pinMode(7, INPUT_PULLUP);

  radio.setAutoAck(false);      // настройка nrf
  radio.setRetries(0, 15);  
  radio.setPayloadSize(32);    
  radio.openWritingPipe(addresses[1]); 
  radio.setChannel(0x60);  

  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_250KBPS); 

  radio.powerUp();
  radio.stopListening();  

  /*pinMode(ch1_trim_plus, INPUT_PULLUP);
  pinMode(ch1_trim_minus, INPUT_PULLUP);
  pinMode(ch2_trim_minus, INPUT_PULLUP);
  pinMode(ch2_trim_plus, INPUT_PULLUP);
  */
  pinMode(2, OUTPUT);
  //pinMode(CH6, INPUT_PULLUP);

 /* while (digitalRead(CH5) == 1) {         //блокировка третьего канала при включенном тумблере ch5(нужно для безопасности при использовании мотора на третьем канале)
    tone(BUZZ, 2000, 200);
    delay(600);
  }
  */
  digitalWrite(2, HIGH);
  tone(BUZZ, 2100, 100);
  delay(100);
}

void loop() {
  //int potValue = analogRead(A0);
  ch1_value = map(analogRead(A1), 0, 1023, 50, 130);
  ch2_value = map(analogRead(A2), 0, 1023, 60, 130);
  potValue[3] = digitalRead(7);
  Serial.println(potValue[3]);
  potValue[0] = ch1_value;
  potValue[1] = ch2_value;
  if (potValue[3] == 0){
    potValue[2] = 0;
  }else if (potValue[3] = 1){
    potValue[2] = 1;
  }
  //potValue[2] = 1; //test
  //int angleValue = map(potValue, 0, 1023, 50, 130);
  radio.write(&potValue, sizeof(potValue));
  Serial.println(potValue[2]);
  Serial.println(potValue[1]);
  delay(10);
  
}
