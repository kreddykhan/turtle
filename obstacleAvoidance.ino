/**
 * Karishma Reddy Khan
 * Script can be loaded on to an Arduino Uno with pins connected as specified to create a bot with IR based obstacle avoidance
 **/

#include <Servo.h>
Servo leftServo; //create a servo object to control the left servo wheel
Servo rightServo; //create a servo object to control the right servo wheel

int leftServoPin=9; //associate the left servo wheel with pin 9
int rightServoPin=6; //associate the right servo wheel with pin 6

int rangeTriggerPinCenter=2; //associate the Trigger of the center distance sensor with pin 2
int rangeEchoPinCenter=3; //associate the Echo of the center distance sensor with pin 2
int rangeTriggerPinRight=4; //associate the Trigger of the right distance sensor with pin 2
int rangeEchoPinRight=5; //associate the Echo of the right distance sensor with pin 2
int rangeTriggerPinLeft=10; //associate the Trigger of the left distance sensor with pin 2
int rangeEchoPinLeft=11; //associate the Echo of the left distance sensor with pin 2

long currentDistanceCenter; //create a variable for the distance sensed by the Center sensor
long currentDistanceRight; //create a variable for the distance sensed by the Right sensor
long currentDistanceLeft; //create a variable for the distance sensed by the Left sensor

void setup(){
  initServos(); //calls the method that initiates the Servos
  
  //declares the Trigger Pins as outputs
  pinMode(rangeTriggerPinCenter, OUTPUT);
  pinMode(rangeTriggerPinRight, OUTPUT);
  pinMode(rangeTriggerPinLeft, OUTPUT);
  
  //declares the Echo Pins as inputs
  pinMode(rangeEchoPinCenter, INPUT);
  pinMode(rangeEchoPinRight, INPUT);
  pinMode(rangeEchoPinLeft, INPUT);
  
  delay(200); //delay by 0.2 seconds
}

void loop(){
  moveBot();
}

//method that determines the current distance to the nearest object
long determineCurrentDistance(int rangeTriggerPin, int rangeEchoPin){
  digitalWrite(rangeTriggerPin, LOW); //first write the Trigger as low to create a clean signal
  delayMicroseconds(2);
  digitalWrite(rangeTriggerPin, HIGH); //release a PING from the Trigger pin
  delayMicroseconds(5);
  digitalWrite(rangeTriggerPin, LOW); //rewrite the Trigger to low after 5 microseconds to end the PING
  
  long duration=pulseIn(rangeEchoPin, HIGH); //use the Echo pin to read the Echo off of the PING
  
  return timeToCentimeters(duration);
}

//method that converts the time based Echo read to a distance
long timeToCentimeters(long time){
  //The speed of sound is 29 microseconds/centimeter
  //The Echo reads the time it takes for the ping to reach an object and come back so we consider half the time.
  return time / 29 / 2;
}

//method that initiates the Servos
void initServos(){
  leftServo.attach(9); //initiates association between left Servo motor and pin 9
  rightServo.attach(6); //initiates association between right Servo motor and pin 6
}

//method that moves the bot forward
void moveForward(){
  leftServo.writeMicroseconds(1700); //moves left wheel counter clockwise
  rightServo.writeMicroseconds(1300); //moves left wheel clockwise
}

//method that turns the bot left
void moveLeft(){
  leftServo.writeMicroseconds(1520); //stops left wheel
  rightServo.writeMicroseconds(1300); //moves right wheel clockwise
}

//method that turns the bot right
void moveRight(){
  leftServo.writeMicroseconds(1700); //moves left wheel counter clockwise
  rightServo.writeMicroseconds(1520); //stops right wheel
}

//method that moves the bot
void moveBot(){
  currentDistanceCenter=determineCurrentDistance(rangeTriggerPinCenter,rangeEchoPinCenter); //determine the distance to the nearest object in the center
  currentDistanceRight=determineCurrentDistance(rangeTriggerPinRight,rangeEchoPinRight); //determine the distance to the nearest object to the right
  currentDistanceLeft=determineCurrentDistance(rangeTriggerPinLeft,rangeEchoPinLeft); //determine the distance to the nearest object to the left
  
  if (currentDistanceCenter<15) { //if the distance to the nearest center object is less than 15cm (which is the bot's diameter)
    if(currentDistanceRight<currentDistanceLeft) { //and if the distance to the right is less than the distance to the left
      moveLeft(); //turn to the left
    } else {
      moveRight(); //otherwise turn to the right
    }
  } else if (currentDistanceRight<15) { //else if the distance to the nearest object to the right is less than 15cm
    moveLeft(); //turn to the left
  } else if (currentDistanceLeft<15) { //else if the distance to the nearest object to the left is less than 15cm
    moveRight(); //turn to the right
  } else {
    moveForward(); //else move forward
  }
}
