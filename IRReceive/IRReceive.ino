#include<avr/io.h>
#include<avr/interrupt.h>

byte readingStart = 0;
volatile uint32_t data = 0;
volatile int nbits = 0;
volatile int debug = 0, debug2 = 0, debug3 = 0;
volatile float debug4 = 0;

void ReceiverInit()
{
  TCCR1A=0;
  TCNT1 = 0;
  TCCR1B = (1 << CS10) | (1 << CS11); //64 prescaler
  TCCR1A = (1 << WGM
  SREG |= (1 << 7); //enable interrupts
  EICRA=(1<<ISC01);   // enable falling edge interrupt.
  EIMSK=(1<<INT0); 
}


ISR(INT0_vect)
{
  //debug = readingStart;
  if (!readingStart && !nbits)
  {
    TCNT1 = 0;
    readingStart = 1;
  }
  else if (readingStart == 1)
  {
     if (TCNT1 >= 250*13 && TCNT1 <= 250*15)  
     {
       readingStart = 2;
       //TCCR1B = (1 << CS10) ; // no prescaler
       TCNT1 = 0;
       data = 0;
     }
   }
   else if (readingStart == 2)
   {
     int t = TCNT1/250;
     //debug3 = t;
     TCNT1 = 0;
     //debug2++;
       //reading the data
       if (t >= 2) // reading 1
       {
         data |= ((uint32_t)1 << (uint32_t)(31-nbits));
       }  
       else  // reading 0
       {
        // do nothing
       } 
       nbits ++;
       if (nbits == 32)
       {
          readingStart = 0; 
       }     
   }
}


int main()
{
  DDRD &= !(1 << 2); //ir receiver pd2
  ReceiverInit();
  Serial.begin(9600);
  while (1)
  {
    /*Serial.print("Debug: ");
    Serial.print(debug);
    Serial.print(" ");
    Serial.print(debug2);
    Serial.print(" ");
    Serial.print(debug3);
    Serial.print(" ");
    Serial.println(debug4);*/
    if (nbits == 32)
    {
      Serial.print("Data: ");
      Serial.println(data, HEX);
      nbits = 0; 
    }
  }  
}
