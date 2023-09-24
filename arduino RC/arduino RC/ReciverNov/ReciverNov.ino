
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
#include <EEPROM.h>


const uint64_t pipeIn = 0xE8E8F0F0E1LL;
#define servo_1 5
#define servo_2 6
#define servo_3 7
#define servo_4 8
#define LED 4
#define FS_SET 2

RF24 radio(9, 10);

bool led_state;
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 
byte resiv_data[6];

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  if (EEPROM.read(0) != 123) {
    EEPROM.write(1, 90);
    EEPROM.write(2, 90);
    EEPROM.write(3, 0);
    EEPROM.write(4, 90);
    EEPROM.write(0, 123);
  }

  // Serial.begin(9600); //открываем порт для связи с ПК

  radio.begin(); 
  radio.setAutoAck(false);     
  radio.setRetries(0, 15);   
  radio.setPayloadSize(32);    
  radio.openReadingPipe(1, pipeIn); 
  radio.setChannel(0x60);  
  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_250KBPS);  

  radio.powerUp(); //начать работу
  radio.startListening(); 


  pinMode(LED, OUTPUT);
  pinMode(FS_SET, INPUT_PULLUP);

  servo1.attach(servo_1);
  servo2.attach(servo_2);
  servo3.attach(servo_3);
  servo4.attach(servo_4);
}

void loop(void) {

  while ( radio.available()) { // слушаем эфир со всех труб
    radio.read( &resiv_data, sizeof(resiv_data) );         // чиатем входящий сигнал
    delay(5);
    /*
      Serial.print("CH1: "); Serial.print(resiv_data[0]); Serial.print(" ");
      Serial.print("CH2: "); Serial.print(resiv_data[1]); Serial.print(" ");
      Serial.print("CH3: "); Serial.print(resiv_data[2]); Serial.print(" ");
      Serial.print("CH4: "); Serial.print(resiv_data[3]); Serial.print(" ");
      Serial.print("CH5: "); Serial.println(resiv_data[4]); Serial.print(" ");

      //    */
    servo1.write(resiv_data[0]);
    servo2.write(resiv_data[1]);
    servo3.write(resiv_data[1]);
    //servo3.writeMicroseconds(map(resiv_data[2], 0, 180, 1028, 1832)); //(использовать эту конструкцию при работе с современными регулятами скорости для бк моторов)
    servo4.write(resiv_data[3]);

    if (!digitalRead(FS_SET) == 1) {
      EEPROM.update(1, resiv_data[0]);
      EEPROM.update(2, resiv_data[1]);
      EEPROM.update(3, resiv_data[2]);
      EEPROM.update(4, resiv_data[3]);
      EEPROM.update(5, resiv_data[4]);
      EEPROM.update(6, resiv_data[5]);
      delay(50);
    }

    if (led_state == 0) {
      led_state = 1;
      digitalWrite(LED, led_state);
    }


  }

  servo1.write(EEPROM.read(1));
  servo2.write(EEPROM.read(2));
  servo3.writeMicroseconds(map(EEPROM.read(3), 0, 180, 1028, 1832));
  servo4.write(EEPROM.read(4));

  if (led_state == 1) {
    led_state = 0;
    digitalWrite(LED, led_state);
    delay(250);
  }
  if (led_state == 0) {
    led_state = 1;
    digitalWrite(LED, led_state);
    delay(250);
  }
  Serial.println(led_state);


}
