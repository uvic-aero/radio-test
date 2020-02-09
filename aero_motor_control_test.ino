//(pi*radius[cm])/(600*velocity[cm/s])
//5V to blue
//GND to green
//TODO:
//Add 180 deg turn function
//Improve readability and variable/function names
//break function ie both direction and pwm pins high

//pin setup for motors
const int dirAccw = 8;
const int dirAcw = 9;
const int pwmA = 10;
const int dirBccw = 6;
const int dirBcw = 4;
const int pwmB = 5;

//motor speed 0-1023
int motorValueA = 0;
int motorValueB = 0;

//desired speed in cm/s
float givenSpeedA = 1;
float givenSpeedB = 0;
//desired time interval for encoders
unsigned long givenTimeA = 0;
unsigned long givenTimeB = 0;
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
unsigned long actualTimeA = 0;
unsigned long actualTimeB = 0;

void setup() {
  pinMode(dirAccw, OUTPUT);
  pinMode(dirAcw, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(dirBccw, OUTPUT);
  pinMode(dirBcw, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(3), EncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(2), EncoderB, RISING);
}

//calls micros() when interrupt is triggered, keeps previous micros() value for calculating time interval of encoder
void EncoderA() {
  timeApre = timeAcur;
  timeAcur = micros();
  encoderStateA = true;
}
void EncoderB() {
  timeBpre = timeBcur;
  timeBcur = micros();
  encoderStateB = true;
}

//sets motor speed and direction, direction is single bool, motor speed is scaled from digital input to analog output (0-1023 -> 0-255)
void motorA(bool dirA, int motorValueA) {
  digitalWrite(dirAccw, dirA);
  digitalWrite(dirAcw, !dirA);
  motorValueA = map(motorValueA, 0, 1023, 0, 255);
  analogWrite(pwmA, motorValueA);
}
void motorB(bool dirB, int motorValueB) {
  digitalWrite(dirBccw, dirB);
  digitalWrite(dirBcw, !dirB);
  motorValueB = map(motorValueB, 0, 1023, 0, 255);
  analogWrite(pwmB, motorValueB);
}

//takes desired speed in cm/s and converts to encoder time interval
void speedA(float givenSpeedA) {
  givenTimeA = 23.562 / (givenSpeedA / 60);
}
void speedB(float givenspeedB) {
  givenTimeB = 23.562 / (givenSpeedB / 60);
}

void loop() {
  //checks if interrupt has been triggered
  if (encoderStateA) {
    actualTimeA = timeAcur - timeApre;
    encoderStateA = false;
  }
  if (encoderStateB) {
    actualTimeB = timeBcur - timeBpre;
    encoderStateB = false;
  }
  //calculates desired encoder time interval based on desired speed in cm/s
  speedA(givenSpeedA);
  speedB(givenSpeedB);
  Serial.println(actualTimeA);
  //if the desired encoder time interval is more than the actual encoder time interval increases motor speed by 10, and vice versa
  if (givenTimeA > actualTimeA) {
    motorValueA += 1;
  }
  else if (givenTimeA < actualTimeA) {
    if (motorValueA != 0) {
      motorValueA -= 1;
    }
  }
  if (givenTimeB > actualTimeB) {
    motorValueB += 1;
  }
  else if (givenTimeB < actualTimeB) {
    if (motorValueB != 0) {
      motorValueB -= 1;
    }
  }

  //sets motor speed and directions
  motorA(dirA, motorValueA);
  motorB(dirB, motorValueB);
}
