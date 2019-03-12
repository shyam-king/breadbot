
int sensors[5] = {
  //left to right
  A0, A1, A2, A3, A4 
};

//motor
int leftMotor[2] = {}, rightMotor[2] = {};


void setup()
{
  for (int i = 0; i < 5; i++)
    pinMode(sensors[i], INPUT);
  Serial.begin(9600);
}

void loop()
{
  for (int i  =0; i < 5; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(digitalRead(sensors[i]));
    Serial.println("");
  }  
  Serial.println("");
  delay(100);
}
