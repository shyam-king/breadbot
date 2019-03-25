
int sensors[5] = {
	//left to right
	A0, A1, A2, A3, A4 
};

int currentPosX = 0;
int currentPosY = 0;
int targetPosX = 0;
int targetPosY = 0;
int dirX = 0; //1 0 -1
int dirY = 0; // 1 0 -1

//motor
int leftMotor[2] = {}, rightMotor[2] = {};

void readSensors()
{
	for (int i = 0; i < 5; i++)
	{
		digitalRead(sensors[i]);
	}  
}

void readTarget();

void setup()
{
	for (int i = 0; i < 5; i++)
		pinMode(sensors[i], INPUT);
	Serial.begin(9600);

	readTarget(); //set targetPosX and targetPosY
}

void traverse()
{
	//encountering a  junction
	if (
		sensors[0] == 1 &&
		sensors[1] == 1 &&
		sensors[2] == 1 &&
		sensors[3] == 1 &&
		sensors[4] == 1 
		){
			currentPosX += dirX;
			currentPosY += dirY;
		}
	else if (
		sensors[0] + sensors[1] + sensors[2] > 1 // left	
	){
		//turn a bit right
	}
	else if (
		sensors[3] + sensors[4] + sensors[2] > 1 // right
	){
		//turn a bit left
	}

	if (currentPosX == targetPosX && currentPosY == targetPosY) // reached
	{
		//stop 
	}
	else if (dirY && targetPosY == currentPosY) // traversing vertically and reached targetY
	{
		dirX = (currentPosX > targetPosX) ? -1 : 1;
		if (dirY * dirX == 1) 
			//turn right
		else 
			//turn left

		dirY = 0;
	}
	else if (dirX && targetPosX == currentPosX) // traversing horizontally and reached target X
	{
		dirY = (currentPosY > targetPosY) ? -1 : 1;
		if (dirY * dirX == 1) 
			//turn right
		else 
			//turn left

		dirX = 0;
	}
}

void loop()
{
	readSensors();
	traverse();

}
