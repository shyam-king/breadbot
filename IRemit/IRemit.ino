#include<avr/io.h>
#include<avr/interrupt.h>

//IR LEDs at PD2

int msticks = 250;

void EmitterInit()
{
  TCCR1A=(1<<WGM01);
  TIMSK1=(1<<OCIE0A);                    //Timer0 Interrupt on Compare Match A                       //enable global interrupts
  DDRD=0b00000100;                       //LED Pin output
  PORTD=0x00;
}


void sendData(byte b)
{

  //start bit
      TCNT1 = 0;
      OCR1A = msticks*3; //3ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
  
  

  for (int i = 0 ; i < 8;i++){
    if (b & (1 << i)){
      
      //send 1
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
     
    } else {
      
      //send 0
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      TCNT1 = 0;
      OCR1A = msticks*2; //2ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      
    }
  }
  //complement
  for (int i = 0 ; i < 8;i++){
    if (!(b & (1 << i))){
      
      //send 1
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
     
    } else {
      
      //send 0
      TCNT1 = 0;
      OCR1A = msticks; //1ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      TCNT1 = 0;
      OCR1A = msticks*2; //2ms
      PORTD ^= 4;
      TCCR1B = (1 << CS10) | (1 << CS11);
      while(TCCR1B & ((1 << CS10) | (1 << CS11)));
      
    }
  }
}

int main()
{
	EmitterInit();
	Serial.begin(9600);
	//TCCR1B = (1 << CS00) | (1 << CS01);
	sei();

        while(1) {sendData(65);delay(100);}
        return 0;
}
