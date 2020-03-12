//(pi*radius[cm])/(600*velocity[cm/s])

//wire setup for encoders
//5V to blue
//GND to green

//motor 1 = A in code     motor 2 = B in code

//pwm motor value|encoder time interval
//            250|350
//            200|370
//            150|410
//            100|500
//             50|950

//--------------------
//TODO:
//Improve readability and variable/function names
//--------------------

//values for error calc
float errorA; 
float errorB; 
float K_pA;
float K_pB;
float P_outputA; 
float P_outputB; 

//pin setup for motors
#define MOTOR_A    1
#define MOTOR_A_CCW 6
#define MOTOR_A_CW  4
#define PWM_A   5

#define MOTOR_B 0
#define MOTOR_B_CCW 8
#define MOTOR_B_CW  9
#define PWM_B   10

//motor speed 0-255
int motorValueA = 0;
int motorValueB = 0;

//desired speed in cm/s
//float givenSpeedA = 5;
//float givenSpeedB = 0;

//desired time interval for encoders
int givenTimeA = 800;
int givenTimeB = 400;

//desired direction
bool dirA = false;
bool dirB = true;

//check for if interrupt has been triggered recently
bool encoderStateA = false;
bool encoderStateB = false;

//current/previous encoder time intervals
unsigned long timeAcur = 0;
unsigned long timeBcur = 0;
unsigned long timeApre = 0;
unsigned long timeBpre = 0;
//actual time interval for encoders
unsigned long actualTimeA = 1000;
unsigned long actualTimeB = 1000;

void setup()
{
  pinMode(MOTOR_A_CCW, OUTPUT);
  pinMode(MOTOR_A_CW, OUTPUT);
  pinMode(PWM_A, OUTPUT);

  pinMode(MOTOR_B_CCW, OUTPUT);
  pinMode(MOTOR_B_CW, OUTPUT);
  pinMode(PWM_B, OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);

  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3), EncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(2), EncoderB, RISING);
}

void loop()
{

  //the arduino has a 64 byte serial buffer so dont send more bytes than that before processing them or else bytes will be lost, sometimes it may need to be flushed/emptied this can be done as follows
  /*
    while (Serial.available())
    Serial.read();
  */

  //original code for revceiving serial bytes from a python program
  //received bytes were a single string eg "R F2 B' U' F U D' R2 D' F' B2 U' R2 U R2 U' B2 R2 U'"
  //this got divided at each space into an array of strings, each entry was a motor command where the letter denoted which motor to rotate, the number denoted how many times to rotate and the apostrophe denoted opposite rotation
  //eg "F2" rotated the front motor 90 degrees cw 2 times, "U" rotated the up motor 90 degrees cw 1 time, "D'" rotated the down motor 90 degrees ccw
  /*
    char cubei;
    String cubef[19];
    int a = 0;
    //only enters when some serial value is received
    while (Serial.available()) {
    //temporarily save the current byte received
    cubei = Serial.read();
    //if the byte is alphanumeric save the byte to the current array index
    if (isAlphaNumeric(cubei)) {
      cubef[a] += cubei;
    }
    //if the byte is a space move 1 index forward
    else if (isSpace(cubei)) {
      a++;
    }
    //if the byte was anything else ie \0 null terminator exit the while loop
    else {
      break;
    }
    }
  */

  //changes the desired time interval of encoders to a new user inputed value
  //not robust at all for now, only use numbers or else it might break
  //give a number within ~300 (max speed) to ~1000 (min speed)
  //conditions for start/end characters can be added just vaguely keep these 3 lines and will it should work
  String temp = "";
  while (Serial.available()) {
    //pretty sure this just appends the next value to the string, idk if its good practise or not
    temp += Serial.read();
  }
  givenTimeA = temp.toInt();
  givenTimeB = temp.toInt();

  //calculates the new time interval of encoders
  interruptCheck();
  //calculates desired encoder time interval based on desired speed in cm/s
  speedCalc();
  //sets motor speed and directions
  motorA(dirA, motorValueA);
  motorB(dirB, motorValueB);
}

//--------------------------

//calls micros() when interrupt is triggered, keeps previous micros() value for calculating time interval of encoder
void EncoderA()
{
  timeApre = timeAcur;
  timeAcur = micros();
  encoderStateA = true;
}
void EncoderB()
{
  timeBpre = timeBcur;
  timeBcur = micros();
  encoderStateB = true;
}

//sets motor speed and direction, direction is single bool, motor speed is scaled from digital input to analog output (0-1023 -> 0-255)
void motorA(bool dirA, int motorValueA)
{
  digitalWrite(MOTOR_A_CCW, dirA);
  digitalWrite(MOTOR_A_CW, !dirA);
  //motorValueA = map(motorValueA, 0, 1023, 0, 255);
  analogWrite(PWM_A, motorValueA);
}
void motorB(bool dirB, int motorValueB)
{
  digitalWrite(MOTOR_B_CCW, dirB);
  digitalWrite(MOTOR_B_CW, !dirB);
  //motorValueB = map(motorValueB, 0, 1023, 0, 255);
  analogWrite(PWM_B, motorValueB);
}

//takes desired speed in cm/s and converts to encoder time interval
void speedCalc()
{
	//convert givenSpeedA and givenSpeedB to encoder time intervals
	givenTimeA = 106.68 / (givenSpeedA / 60);
	givenTimeB = 106.68 / (givenSpeedB / 60);
	
	//calculate error between desired and actual time
	errorA = givenTimeA - actualTimeA;
	errorB = givenTimeB - actualTimeB;
	
	//multiply error by proportional coefficient K_p
	P_outputA = K_pA * errorA;
	P_outputB = K_pB * errorB;
	
	if(P_outputA >= 0)
	{
	givenTimeA = givenTimeA + P_outputA; 
	}
	else
	{
	givenTimeA = givenTimeA - P_outputA;
	}
	
	if(P_outputB >= 0)
	{
	givenTimeB = givenTimeB + P_outputB;
	}
	else
	{
	givenTimeB = givenTimeB - P_outputB;
	}
}

void interruptCheck()
{
  //checks if interrupt has been triggered
  if (encoderStateA)
  {
    actualTimeA = timeAcur - timeApre;
    encoderStateA = false;
  }
  if (encoderStateB)
  {
    actualTimeB = timeBcur - timeBpre;
    encoderStateB = false;
  }
}

void brake(int length)
{
  digitalWrite(MOTOR_A_CCW, HIGH);
  digitalWrite(MOTOR_A_CW, HIGH);
  digitalWrite(PWM_A, HIGH);

  digitalWrite(MOTOR_B_CCW, HIGH);
  digitalWrite(MOTOR_B_CW, HIGH);
  digitalWrite(PWM_B, HIGH);
  delay(length);
}

//turns the robot 180 degrees
void turn()
{
  for (int i = 0; i < 100; i++) {
    digitalWrite(MOTOR_A_CCW, true);
    digitalWrite(MOTOR_A_CW, false);
    analogWrite(PWM_A, 255);
    digitalWrite(MOTOR_B_CCW, false);
    digitalWrite(MOTOR_B_CW, true);
    analogWrite(PWM_B, 255);
    delay(1);
  }
  analogWrite(PWM_A, 0);
  analogWrite(PWM_B, 0);
}
