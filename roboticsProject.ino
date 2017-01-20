#include <QTRSensors.h>


#define NUM_SENSORS   4     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   QTR_NO_EMITTER_PIN     // emitter is controlled by digital pin 2

const int spd = 255;
const float slowAmount = 0.75f;

bool isDone;

QTRSensorsRC qtrrc((unsigned char[]) {2, 3, 22, 23},NUM_SENSORS, TIMEOUT, EMITTER_PIN); //pins 22, 23 - left 2, 3 - Right
unsigned int sensorValues[NUM_SENSORS];
int pwm1 = 9, a = 8, b = 7, pwm2 = 16, c = 17, d = 18;

void setup(){
  pinMode(pwm1, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  isDone = false;
  drive();
  goStraight();
}


void loop(){
  while (!isDone){ //continues to move until finished following the line
    qtrrc.read(sensorValues);
    if (sensorValues[0] < 2000 && sensorValues[1] < 2000 && sensorValues[2] < 2000 && sensorValues[3] < 2000){
      isDone = true;
      break;
    }
    else if (sensorValues[0] < 2000 || sensorValues[3] < 2000) goLeft();
    else if (sensorValues[1] < 2000 || sensorValues[2] < 2000) goRight();
    else goStraight();
  }
  stopDrive();
}

void drive(){
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
}

void driveReverse(){
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
}

void stopDrive(){
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
}

void goStraight(){ //keep speed constant at highest
  analogWrite(pwm1, spd);
  analogWrite(pwm2, spd);
}

void moveAtSpeed(float speedScaleFactor){ //0.0 <= speedScaleFactor <= 1.0
  analogWrite(pwm1, speedScaleFactor*spd);
  analogWrite(pwm2, speedScaleFactor*spd);
}

void goLeft(){
  analogWrite(pwm1, spd);
  analogWrite(pwm2, slowAmount*spd); //slow down left wheel to turn left
}

void goRight(){
  analogWrite(pwm1, slowAmount*spd); //slow down right wheel to turn right
  analogWrite(pwm2, spd);
}
