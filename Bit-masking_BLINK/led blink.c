#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA = DDRA & ~(1<<PINA0);// PORTA.0 - IP,  DDRA.0 = 0
	DDRA = DDRA | (1<<PINA1); //PORTA.1 - O/p, DDRA.1 = 1
	PORTA = PORTA | (1<<PINA0);// 1 to PORTA.0, enable the pull up register
    while (1) 
    {
		while((PINA & (1<<PINA0))==0)// checking for PINA0 for 0 
        // when switch pressed at PINA0 causes logic low from the low impedance path to ground 
		{// BLINK
			PORTA = PORTA | (1<<PINA1);// PORTA.1 = 1, LED ON
			_delay_ms(200);
			PORTA = PORTA & ~(1<<PINA1);// PORTA.1 = 0, LED OFF
			_delay_ms(200);
		}
    }
}