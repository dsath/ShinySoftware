#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h> 
#include <stdio.h>

#define baud 9600
#define myubrr F_CPU/16/baud - 1

#define OUTPORT PORTB
#define OUTDDR DDRB 

void USART_Init( unsigned int ubrr);
char USART_Receive( void ); 

int main()  {
  char data;

  DDRD  = 0;
  PORTD = 0;

  OUTPORT = ~0;      // configure Port B pins for input/output
  OUTDDR = ~0;     // set initial value for Port B outputs
  
  USART_Init(myubrr);

  while(1) {

    OUTPORT = OUTPORT & 0b11111110;
    _delay_ms(200);
    _delay_ms(200);
    OUTPORT = OUTPORT | 0b00000001;
    _delay_ms(200);
    _delay_ms(200);

    if( PIND == 0b11111110 ) {
        OUTPORT = OUTPORT & 0b00111111;
        _delay_ms(200);
        _delay_ms(200);
        OUTPORT = OUTPORT | 0b11000000;
        _delay_ms(200);
        _delay_ms(200);
    }

    if ( UCSR0A & (1<<RXC0) ) {
      data = UDR0;
      if ( data == 'A') {
        data = USART_Receive();
        if(data == 'B') {
          //If B recieved, soft reset and continue
          OUTPORT = OUTPORT & 0b00111111;
          _delay_ms(200);
          _delay_ms(200);
          OUTPORT = OUTPORT | 0b11000000;
          _delay_ms(200);
          _delay_ms(200);
        } else {
          //Check for errors
          OUTPORT = 0b01000000;
          while(1);
        }
      } else {
        //Check for errors
        OUTPORT = 0b10000000;
        while(1);
      }
    }
  }
  return 0;
}

void USART_Init( unsigned int ubrr) {
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8); //clears UBRR0H
  UBRR0L = (unsigned char)ubrr;
  //Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
}

char USART_Receive( void ) {
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}

void USART_Receive_5( char* d ) {
  /* Wait for data to be received */
  for(int i = 0; i < 5; i++) {
    while ( !(UCSR0A & (1<<RXC0)) );
    d[i] = UDR0;
  }
}
