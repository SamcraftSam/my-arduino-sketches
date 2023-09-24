#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/sleep.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(0, INPUT);
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  delay(100);
  digitalWrite(1, LOW);
  delay(100);
}

void power_off()
{
  delay(10000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(0)==HIGH)
  {
    for (int i = 0; i < 5; i++){
      digitalWrite(1, HIGH);
      _delay_ms(1000);
      digitalWrite(1, LOW);
      _delay_ms(1000);
    }
    power_off();
    //digitalWrite(1, HIGH);
  }
  else
  {
    digitalWrite(1, LOW);
  }
}
