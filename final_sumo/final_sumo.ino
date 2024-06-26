#include <Servo.h>

Servo myServo; 

const int speed = 70;
const int speedVariable = 20;

const int min_distance_sumo = 30;
const int min_distance_search = 15;

// Line tracker sensor
const int rightSens = A0;
const int centerSens = A1;
const int leftSens = A2;

// H-bridge

// Right motor
const int in1 = 3;
const int in2 = 2;
const int ena = 11;

// Left motor
const int in3 = 13;
const int in4 = 12;
const int enb = 5;

// Ultrasonic sensor
const int echoPin = 6;
const int trigPin = 7;

const int dipSwitchPin = 4;
bool isSumoMode = false;

void setup()
{

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(rightSens, INPUT);
  myServo.attach(9);

  pinMode(dipSwitchPin, INPUT_PULLUP);

  Serial.begin(9600);
}

int counter = 0;

void loop() {
  if(millis() > 5000)
  {
    if (digitalRead(dipSwitchPin) == LOW) {
    // Dip switch is on, set sumo mode
      isSumoMode = true;
    } else {
      // Dip switch is off, set search mode
      isSumoMode = false;
    }

    // Perform corresponding behavior based on the dip switch state
    if (isSumoMode) {
      sumo();
    } else {
      search();
    }
  }
}

void sumo()
{
  int rightSensorValue = analogRead(rightSens);
  int centerSensorValue = analogRead(centerSens);
  int leftSensorValue = analogRead(leftSens);
  int threshold = 500;

  if(rightSensorValue < threshold && leftSensorValue < threshold)
  {   
    // Line detected
    backward();
    delay(100);
  }
  else
  {
    // No line
    int object_distance = getDistance();
    if(object_distance <= min_distance_sumo)
    {
      turnRight();
      counter++;
      if(counter >= 2)
      {
        forward();
      }
    }
    else
    {
      counter = 0;
      forward();
      turnLeft();
      delay(300);
    }
  }
}

void search()
{
  int distance = getDistance();
  int list[3];
  forward();
  if (distance <= min_distance_search)
  {
    stopMotors();
    delay(200);
    myServo.write(0);   
    delay(200);
    list[0] = getDistance();
    delay(100);
    myServo.write(180);
    delay(200);
    list[1] = getDistance();
    delay(100);
    myServo.write(90);
    delay(1000);
    list[2] = getDistance();
    delay(100);
    

    if(list[0] >= min_distance_search && list[0] > list[1])
    {
      turnRight();
      delay(500);
    }
    else if(list[1] >= min_distance_search && list[1] > list[0])
    {
      turnLeft();
      delay(500);
    }
  }
}

int getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, 30000);

  return (duration * 0.0343 / 2);
}

void stopMotors()
{
  analogWrite(ena, 0);
  analogWrite(enb, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


void backward()
{
  analogWrite(ena, speed);
  analogWrite(enb, speed + speedVariable);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void forward()
{
  analogWrite(ena, speed);
  analogWrite(enb, speed + speedVariable);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void turnRight()
{
  analogWrite(ena, speed);
  analogWrite(enb, speed + speedVariable);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turnLeft()
{
  analogWrite(ena, speed);
  analogWrite(enb, speed + speedVariable);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
