#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

void setup() {
  // put your setup code here, to run once:
  DDRB = 0x02;
  PORTB = 0x00;
  PORTB |= (1<<PB1);
  _delay_ms(100); 
  PORTB &= ~(1<<PB1);
  _delay_ms(1000);
  PORTB |= (1<<PB1);
  _delay_ms(1000); 
  PORTB &= ~(1<<PB1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((PINB&(1<<PINB0)) == 1)  // If the Button is pressed, the PB0 goes LOW
  {
    for (int i; i<10; i++){
      PORTB |= (1<<PB1);
      delay(1000); 
      PORTB &= ~(1<<PB1);
      delay(1000);  
    }
    PORTB |= (1<<PB1);
    //_delay_ms(1000);
  }
  //PORTB &= ~(1<<PB1);
  else 
  {
    PORTB &= ~(1<<PB1);
    //PORTB |= (1<<PB1);
  }
}
