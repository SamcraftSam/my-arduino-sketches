/* CODE for invertors buzzer interrupt module
  Created by: Alexander @SamcraftSam (GitHub)
*/
#define F_CPU 1000000UL //decreased to 1 MHz to reduce power
#include<avr/io.h>
#include<avr/interrupt.h>

/*
 * APPROX. TIMER CALCULATIONS:
 * 
 * 1 000 000 / 1024 = 976 Hz   
 * T = 1 / 976 = 0,001024
 * 0 to 255 - 0,001024 * 255 = 0,26112
 * N = 1000 / 26,112 ~= 38 (precision is not important in our case)
 * wee need to run timer 38 times for 1044 ms delay!
 * 
 * our events timeout - 10 sec. 10000 / 26,112 = 383 iterations
 * 
 *in practice, this timeout increased to 12-13 seconds because of 
 *approximate calculations and imprecise interrupt methods
 */
 
#define ITERS 383
#define PERIOD 26.112
#define BUZZER_DELAY 300

ISR (PCINT0_vect)        // Interrupt service routine 
{
  MCUCR&=~(1<<SE);      //Disabling sleep mode inside interrupt routine
}

void external_interrupt()
{
  DDRB |= (1<<PB1);     // set PB1 as output(LED)
  sei();                //enabling global interrupt
  // timer implementation! May break the code!! UPD: OK, code working!
  TCCR0A-0x00;
  TCCR0B=0x00;
  TCCR0B |= (1<<CS00)|(1<<CS02);
  TCNT0=0;
  //
  GIMSK|= (1<<PCIE);    //Pin change interrupt enable
  PCMSK|=(1<<PCINT0);   //Pin change interrupt to pin PB0
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
    waitmillis(BUZZER_DELAY);
    PORTB&=~(1<<PB1);
    waitmillis(BUZZER_DELAY); 
  }
}



void check_input()
{
  unsigned int i = 0;
  while(i<ITERS)
  {
    while((TIFR & (1 << TOV0)) == 0);
    TIFR |= (1<<TOV0);
    if ((PINB&(1<<PINB0)) == 1) {
      i = 0; //resetting timer on external event
    }
    else 
    {
      i++;  
    }
    
  }
}

void waitmillis(int num)
{
  unsigned int j = 0;
  while(j<(num/PERIOD)) //computing number of iters 
  {
    while((TIFR & (1 << TOV0)) == 0);
    TIFR |= (1<<TOV0);
    j++;
  }
}

int main()
{
  external_interrupt();
  while(1)
  {
    enters_sleep();        // configuration for power down sleep mode
    warn();
    check_input();
  }
}
