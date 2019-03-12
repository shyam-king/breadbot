#include<avr/io.h>
#include<avr/interrupt.h>

volatile byte readingStart = 0;
volatile byte data = 0;
volatile int nbits = 0;

void ReceiverInit()
{
  TCCR1A=0;
  TCNT1 = 0;
  EICRA=(1<<ISC01);                               // enable falling edge interrupt.
  EIMSK=(1<<INT0); 
}


ISR(INT0_vect)
{
  if (!readingStart && !nbits)
  {
    TCNT1 = 0;
    TCCR1B = (1 << CS10) | (1 << CS11);
    readingStart = 1;  
  }
  if (readingStart == 1)
  {
     if (TCNT1 >= 250*4 - 10 && TCNT1 <= 1010)  
     {
       readingStart = 2;
       TCNT1 = 0;
       data = 0;
       nbits = 0;
     }
     else
     {
       readingStart = 0;
       TCNT1 = 0;
       TCCR1B = 0;
       data = 0;
       nbits = 0;
     }
   }
   if (readingStart == 2)
   {
       //reading the data
       if (TCNT1 >= 250 *2 - 10 && TCNT1 <= 510)
       {
         data |= (1 << nbits);
         nbits ++;
       }      
       else if (TCNT1 < 490 || TCNT1 > 760)
       {
         readingStart = 0;
         TCNT1 = 0;
         TCCR1B = 0;
         data = 0;
         nbits = 0;
       }
       
   }
}


int main()
{
  DDRD &= !(1 << 2); //ir receiver pd2
  ReceiverInit();
  sei();
  Serial.begin(9600);
  while (1)
  {
    if (nbits == 8)
    {
      Serial.println(data);
      nbits = 0; 
    }
  }  
}
