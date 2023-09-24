#define F_CPU 1000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

ISR (PCINT0_vect)        // Interrupt service routine 
{
  MCUCR&=~(1<<SE);      //Disabling sleep mode inside interrupt routine
}

void external_interrupt()
{
  DDRB |= (1<<PB1);     // set PB1 as output(LED)
  sei();                //enabling global interrupt
  GIMSK|= (1<<PCIE);    //Pin change interrupt enable
  PCMSK|=(1<<PCINT0);   //Pin change interrupt to 2nd pin PB2
}

void enters_sleep()
{  
  MCUCR|=(1<<SM1);      // enabling sleep mode and powerdown sleep mode
  MCUCR|= (1<<SE);     //Enabling sleep enable bit
  __asm__ __volatile__ ( "sleep" "\n\t" :: ); //Sleep instruction to put controller to sleep
  //controller stops executing instruction after entering sleep mode  
}

void warn()
{
  for (int i = 0; i < 10; i++)
  {
    PORTB |= (1<<PB1);
    _delay_ms(2000);
    PORTB&=~(1<<PB1);
    _delay_ms(2000); 
  }
}

void check_input()
{
  
}

int main()
{
  external_interrupt();
  while(1)
  {
    enters_sleep();        // configuration for power down sleep mode
    warn();
    _delay_ms(1000);
  }
}
