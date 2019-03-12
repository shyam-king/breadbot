#include<avr/io.h>
#include<avr/interrupt.h>

#define BIT_START 0
#define BIT_1 1
#define BIT_0 2
#define BIT_BURST 3
#define timer1 ((1 << WGM12) | (1 << CS11) | (1 << CS10))

//IR LEDs at PD2

volatile int debug= 0;
int LED = (1 << 2);

void Init()
{
  //global interrupts
  SREG |= (1 << 7);
  
  //timers
  TCCR0A=(1<<WGM01); //timer0 CTC
  OCR0A = 210; // 38Khz transmission
  TIMSK0=(1<<OCIE0A);//Timer0 Interrupt on Compare Match A      
  
  TCCR1B |= (1 << WGM12); //timer 1 CTC
  TIMSK1 = (1<<OCIE1A); //timer 1 interrupt
  
  TCCR2A = (1 << WGM21); //timer2 CTC
  TIMSK2 = (1 << OCIE2A); //timer 2 interrupt

  //IR LED  
  DDRD |= LED; //set output
  PORTD &= !LED; //turn it off
  
  Serial.begin(9600);
}

void IR_emit_ms(float ms)
{
  TCCR0B = (1 << CS00); //no prescaler
  TCNT0 = 0;
  TCCR1B = timer1; // 64 prescalar and ctc
  OCR1A = 250 * ms; //250 ticks of 64 prescaler is 1 ms
  TCNT1 = 0;
  PORTD |= LED; //turn IR on
}

void IR_emit_us(float us)
{
  TCCR0B = (1 << CS00); //no prescaler
  TCNT0 = 0;
  TCCR1B = (1 << CS00); // no prescalar
  OCR1A = 16 * us; //250 ticks of 64 prescaler is 1 ms
  TCNT1 = 0;
  PORTD |= LED; //turn IR on
}

void IR_stop()
{
  TCCR0B = 0;//stop timer 0
  PORTD &= !LED; //turn off IR
}

ISR(TIMER0_COMPA_vect)
{
  PORTD ^= LED; //toggle (50% duty cycle) 
}

ISR(TIMER1_COMPA_vect)
{
  TCCR1B = 0;//stop timer 1
  IR_stop();//stop emitting
}

ISR(TIMER2_COMPA_vect)
{
   TCCR2B = 0; 
}

byte IR_emitting()
{  
  return TCCR1B;  
}

void wait_ms(float ms)
{
  TCCR2B = (1 << CS21) | (1<<CS20);
  OCR2A = 250 * ms;
  TCNT2 = 0;
  while (TCCR2B);
}

void wait_us(float us)
{
  TCCR2B = (1 << CS20);
  OCR2A = 16 * us;
  TCNT2 = 0;
  while (TCCR2B);
}

void sendPulse(byte Bit)
{
  switch(Bit)
  {
     case BIT_START: 
       IR_emit_ms(9); //9ms HIGH
       while(IR_emitting()); //wait
       wait_ms(4); //1ms LOW
     break;
     
     case BIT_1: 
       IR_emit_ms(1); //2us HIGH
       while(IR_emitting()); //wait
       wait_ms(2.5); //1us LOW
     break;
     
     case BIT_0: 
       IR_emit_ms(1); //1us HIGH
       while(IR_emitting()); //wait
       wait_ms(1.25); //1us LOW
     break;
     
     case BIT_BURST:
       IR_emit_ms(20);
       while(IR_emitting()); //wait
  }
}

void sendData(uint32_t d)
{
  //sendPulse(BIT_BURST);
  
  for(int i = 0; i < 32; i++)
  {
    if (d & 1) // right most bit
    {
      sendPulse(BIT_1);
    }
    else
    {
      sendPulse(BIT_0); 
    }
    d >>= 1;
  } 
}

int main()
{
  Init();
  
  while(1) {
    sendPulse(BIT_START);
    sendData(0);
    sendPulse(BIT_BURST);
    wait_ms(100);
  }
  return 0;
}
