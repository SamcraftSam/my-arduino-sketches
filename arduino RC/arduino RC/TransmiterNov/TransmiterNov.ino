
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <EEPROM.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;

#define RATE 65               // расходы в процентах (процент отколонения сервоприводов RATE 100  -  180 градусов)
#define TIMELOOP 10           // время обновления значений с джойстиков и тумблеров в миллисекундах 

#define BUZZ 3               // порт подключения пищалки

#define CH1 1                // порты подключения стиков к аналоговым входам (A0, A1, A4,A5)
#define CH2 0
#define CH3 5
#define CH4 4

#define CH5 2                // порты подключения двух дискретных каналов
#define CH6 5

#define ch1_trim_minus A3    // подключение кнопок триммеров
#define ch1_trim_plus A2
#define ch2_trim_minus 7
#define ch2_trim_plus 8

RF24 radio(9, 10); 
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 
byte transmit_data[6];

byte ch1_value;              // переменные для каналов
byte ch2_value;
byte ch3_value;
byte ch4_value;
bool ch5_value;
bool ch6_value;

int8_t ch1_trim;             // переменные для работы триммеров
int8_t ch2_trim;
bool ch1_flag_plus;
bool ch1_flag_minus;
bool ch2_flag_plus;
bool ch2_flag_minus;

bool ch1_flag_center = 1;
bool ch2_flag_center = 1;

uint32_t last_time = millis();


void setup() {
  //Serial.begin(9600); 

  if (EEPROM.read(0) != 123) { 
    EEPROM.write(1, 22);
    EEPROM.write(2, 22);
    EEPROM.write(0, 123);

  }

  delay(10);


  radio.begin();           // запуск модуля nrf
  delay(1000);



  radio.setAutoAck(false);      // настройка nrf
  radio.setRetries(0, 15);  
  radio.setPayloadSize(32);    
  radio.openWritingPipe(my_radio_pipe); 
  radio.setChannel(0x60);  

  radio.setPALevel (RF24_PA_MAX);
  radio.setDataRate (RF24_250KBPS); 

  radio.powerUp();
  radio.stopListening();  

  pinMode(ch1_trim_plus, INPUT_PULLUP);
  pinMode(ch1_trim_minus, INPUT_PULLUP);
  pinMode(ch2_trim_minus, INPUT_PULLUP);
  pinMode(ch2_trim_plus, INPUT_PULLUP);
  pinMode(CH5, INPUT_PULLUP);
  pinMode(CH6, INPUT_PULLUP);

 /* while (digitalRead(CH5) == 1) {         //блокировка третьего канала при включенном тумблере ch5(нужно для безопасности при использовании мотора на третьем канале)
    tone(BUZZ, 2000, 200);
    delay(600);
  }
  */

  tone(BUZZ, 2100, 100);
  delay(100);
}

void loop(void) {

  trimer();                              // функция для работы триммера

  if ((millis() - last_time) >= TIMELOOP) {
    last_time = millis();
    dataRead();
  }



  if (ch1_value >= 180) ch1_value = 179;
  if (ch2_value >= 180) ch2_value = 179;
  if (ch1_value <= 0) ch1_value = 0;
  if (ch2_value <= 0) ch2_value = 0;

/*
  if (ch5_value == 1) ch3_value = 0;  //блокировка третьего канала при включенном тумблере ch5(нужно для безопасности при использовании мотора на третьем канале)
*/

  transmit_data[0] = ch1_value;
  transmit_data[1] = ch2_value;
  transmit_data[2] = ch3_value;
  transmit_data[3] = ch4_value;
  transmit_data[4] = ch5_value;
  transmit_data[5] = ch6_value;

  radio.write(&transmit_data, sizeof(transmit_data));

  /*

    Serial.print("CH2: "); Serial.print(ch2_value); Serial.print(" ");
    Serial.print("CH3: "); Serial.print(ch3_value); Serial.print(" ");
    Serial.print("CH4: "); Serial.print(ch4_value); Serial.print(" ");
    Serial.print("CH5: "); Serial.println(ch5_value); Serial.print(" ");
  */
}

void dataRead() {
    ch1_value = map(analogRead(CH1), 1023, 0, ((100 - RATE) / 2 * 1.8), (1.8 * (RATE + ((100 - RATE) / 2)))) + ch1_trim + 20;
    ch2_value = map(analogRead(CH2), 1023, 0, ((100 - RATE) / 2 * 1.8), (1.8 * (RATE + ((100 - RATE) / 2)))) + ch2_trim + 20;
    ch3_value = map(analogRead(CH3), 0, 1023, 0, 180);
    ch4_value = map(analogRead(CH4), 0, 1023, 0, 180);
    ch5_value = !digitalRead(CH5);
    ch6_value = !digitalRead(CH6);
}

void trimer() {

  if (!digitalRead(ch1_trim_plus) & ch1_flag_plus == 0) {
    ch1_trim++;
    ch1_flag_plus = 1;
    EEPROM.update(1, (ch1_trim + 22));
  }
  if (digitalRead(ch1_trim_plus) & ch1_flag_plus == 1) ch1_flag_plus = 0;

  if (!digitalRead(ch1_trim_minus) & ch1_flag_minus == 0) {
    ch1_trim-- ;
    ch1_flag_minus = 1;
    EEPROM.update(1, (ch1_trim + 22));
  }
  if (digitalRead(ch1_trim_minus) & ch1_flag_minus == 1) ch1_flag_minus = 0;


  if (ch1_trim == 0 & ch1_flag_center == 0)  {
    tone(BUZZ, 2000, 200);
    ch1_flag_center = 1;
  }

  if (ch1_trim != 0 & ch1_flag_center == 1) ch1_flag_center = 0;

  //ограничение триммера

  if (ch1_trim >= 21 ) {
    tone(BUZZ, 1900, 400);
    ch1_trim = 20;
  }

  if (ch1_trim <= -21 ) {
    tone(BUZZ, 1900, 400);
    ch1_trim = -20;
  }

  if (ch2_trim >= 21 ) {
    tone(BUZZ, 1900, 400);
    ch2_trim = 20;
  }

  if (ch2_trim <= -21 ) {
    tone(BUZZ, 1900, 400);
    ch2_trim = -20;
  }


  if (!digitalRead(ch2_trim_plus) & ch2_flag_plus == 0) {
    ch2_trim = ch2_trim + 2;
    ch2_flag_plus = 1;
    EEPROM.update(2, (ch2_trim + 22));
  }
  if (digitalRead(ch2_trim_plus) & ch2_flag_plus == 1) ch2_flag_plus = 0;

  if (!digitalRead(ch2_trim_minus) & ch2_flag_minus == 0) {
    ch2_trim = ch2_trim - 2 ;
    ch2_flag_minus = 1;
    EEPROM.update(2, (ch2_trim + 22));
  }
  if (digitalRead(ch2_trim_minus) & ch2_flag_minus == 1) ch2_flag_minus = 0;



  if (ch2_trim == 0 & ch2_flag_center == 0)  {
    tone(BUZZ, 2000, 300);
    ch2_flag_center = 1;
  }

  if (ch2_trim != 0 & ch2_flag_center == 1) ch2_flag_center = 0;

}
