#include <avr/io.h>
#include <util/delay.h>


#define F_CPU 16000000          
#define BAUD 9600L              
#define UBRR_VALUE ((F_CPU/(BAUD*16))-1) 

#define PIN               PC0

void init_USART() {
    UBRR0H = UBRR_VALUE>>8;             
    UBRR0L = UBRR_VALUE;                
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);    
}

void send_UART(int data) {
    while(!(UCSR0A&(1<<UDRE0))){};         
    UDR0 = data;                  
}

int receive_UART() {
    while(!(UCSR0A&(1<<RXC0)));
    return UDR0;
}


void InitADC()
{
    ADMUX=(1<<REFS0);                         
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); 
}

uint8_t ReadADC(uint8_t ch)
{
   ch=ch&0b00000111;
   ADMUX|=ch;
   ADCSRA|=(1<<ADSC);
   while(!(ADCSRA & (1<<ADIF)));
   ADCSRA|=(1<<ADIF);
   return(ADC);
}


int main(void)
{

    uint8_t result;
    InitADC();

    init_USART();    	

    while(1)
    {
        char buf[10];
        int i = 0;
        result = ReadADC(0);
        sprintf(buf, "%d ", result);
        while(buf[i] != 0) 
        {
            send_UART(buf[i]);
            i++;                             
        }

        _delay_ms(1000);
    }
    return 0;
} 
