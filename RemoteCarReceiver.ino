/**
 * Karishma Reddy Khan
 * Can be loaded on to an Arduino Uno to create a remote control car RF receiver
 **/

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

int CE_Pin= 9; //associate the CE Pin with pin 9
int CS_Pin=10; //associate the CS Pin with pin 10
RF24 radio (CE_Pin, CS_Pin); //associate the CE and CS Pins with the radio
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; //establish the two addresses to be used with the radio

int message; //the initial received message
int returnMessage; //the returned transmission message

#include <Servo.h>
Servo leftServo; //create a servo object to control the left servo wheel
Servo rightServo; //create a servo object to control the right servo wheel

int leftServoPin=7; //associate the left servo wheel with pin 7
int rightServoPin=6; //associate the right servo wheel with pin 6

int rangeTriggerPinCenter=2; //associate the Trigger of the center distance sensor with pin 2
int rangeEchoPinCenter=3; //associate the Echo of the center distance sensor with pin 3
int rangeTriggerPinRight=4; //associate the Trigger of the right distance sensor with pin 4
int rangeEchoPinRight=5; //associate the Echo of the right distance sensor with pin 5
int rangeTriggerPinLeft=14; //associate the Trigger of the left distance sensor with pin 14/A0
int rangeEchoPinLeft=15; //associate the Echo of the left distance sensor with pin 15/A1
int rangeTriggerPinBack=16; //associate the Trigger of the back distance sensor with pin 16/A2
int rangeEchoPinBack=17; //associate the Echo of the back distance sensor with pin 17/A3

long currentDistanceCenter; //create a variable for the distance sensed by the Center sensor
long currentDistanceRight; //create a variable for the distance sensed by the Right sensor
long currentDistanceLeft; //create a variable for the distance sensed by the Left sensor
long currentDistanceBack; //create a variable for the distance sensed by the Back sensor

void setup() {
  Serial.begin(9600);
  
  initServos(); //calls the method that initiates the Servos
  
  radio.begin(); //initialize the radio
  radio.openWritingPipe(pipes[1]); //associate the writing pipe with address 2
  radio.openReadingPipe(1,pipes[0]); //associate the reading pipe with address 1
  radio.startListening(); //start listening for a transmission
  
  /*test LEDs
  pinMode(moveForwardLED, OUTPUT);
  pinMode(moveBackwardLED, OUTPUT);
  pinMode(turnLeftLED, OUTPUT);
  pinMode(turnRightLED, OUTPUT);*/
  
  //declares the Trigger Pins as outputs
  pinMode(rangeTriggerPinCenter, OUTPUT);
  pinMode(rangeTriggerPinRight, OUTPUT);
  pinMode(rangeTriggerPinLeft, OUTPUT);
  pinMode(rangeTriggerPinBack, OUTPUT);
  
  //declares the Echo Pins as inputs
  pinMode(rangeEchoPinCenter, INPUT);
  pinMode(rangeEchoPinRight, INPUT);
  pinMode(rangeEchoPinLeft, INPUT);
  pinMode(rangeEchoPinBack, INPUT);

}

void loop() {
  if (radio.available()) { //if a transmission is available
    radio.read(&message,1); //read the transmission
    if (message == 10) { //if the transmission returns a value of 10
      currentDistanceCenter=determineCurrentDistance(rangeTriggerPinCenter,rangeEchoPinCenter); //determine the distance to the nearest object in the center
      if(currentDistanceCenter>15) { //if the distance is greater than 15cm
        Serial.println("Moving Forward");
        
        /*digitalWrite(moveForwardLED,HIGH);
        digitalWrite(moveBackwardLED,LOW);
        digitalWrite(turnLeftLED,LOW);
        digitalWrite(turnRightLED,LOW);*/
        
        moveForward(); //move the bot forward
        
        radio.stopListening(); //stop listening for transmissions
        returnMessage=11; //establish a return message of value 11 which is received but causes no result in the controller code
        radio.write(&returnMessage,1); //transmit this return message value
        radio.startListening(); //start listening again for transmissions
      } else if(currentDistanceCenter<15) { //else if the distance is less than 15cm
        Serial.println("HELP");
        
        stopMoving(); //stop moving
        
        radio.stopListening(); //stop listening for transmissions
        returnMessage=12; //establish a return message of value 12 which indicates an issue to the controller code
        radio.write(&returnMessage,1); //transmit this return message value
        radio.startListening(); //start listening again for transmissions
      }
    } else if (message == 20) { //else if the transmission returns a value of 20
      currentDistanceBack=determineCurrentDistance(rangeTriggerPinBack,rangeEchoPinBack); //determine the distance to the nearest object behind the bot
      if(currentDistanceBack>15) {
        Serial.println("Moving Backward");
        
        /*digitalWrite(moveForwardLED,LOW);
        digitalWrite(moveBackwardLED,HIGH);
        digitalWrite(turnLeftLED,LOW);
        digitalWrite(turnRightLED,LOW);*/
        
        moveBackward();
        
        radio.stopListening();
        returnMessage=21;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else if(currentDistanceBack<15) {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=22;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 30) { //else if the transmission returns a value of 30
      currentDistanceLeft=determineCurrentDistance(rangeTriggerPinLeft,rangeEchoPinLeft); //determine the distance to the nearest object to the left
      if(currentDistanceLeft>15) {
        Serial.println("Turning Left");
        
        /*digitalWrite(moveForwardLED,LOW);
        digitalWrite(moveBackwardLED,LOW);
        digitalWrite(turnLeftLED,HIGH);
        digitalWrite(turnRightLED,LOW);*/
        
        moveLeft();
        
        radio.stopListening();
        returnMessage=31;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else if(currentDistanceLeft<15) {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=32;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 40) { //else if the transmission returns a value of 40
      currentDistanceRight=determineCurrentDistance(rangeTriggerPinRight,rangeEchoPinRight); //determine the distance to the nearest object to the right
      if (currentDistanceRight>15) {
        Serial.println("Turning Right");
        
        /*digitalWrite(moveForwardLED,LOW);
        digitalWrite(moveBackwardLED,LOW);
        digitalWrite(turnLeftLED,LOW);
        digitalWrite(turnRightLED,HIGH);*/
        
        moveRight();
        
        radio.stopListening();
        returnMessage=41;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else if(currentDistanceRight<15) {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=42;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 50) { //else if the transmission returns a value of 50
      currentDistanceCenter=determineCurrentDistance(rangeTriggerPinCenter,rangeEchoPinCenter); //determine the distance to the nearest object in the center
      currentDistanceLeft=determineCurrentDistance(rangeTriggerPinLeft,rangeEchoPinLeft); //determine the distance to the nearest object to the left
      if(currentDistanceCenter>15 && currentDistanceLeft>15) { //if both these distances are greateer than 15cm
        Serial.println("Moving Diagonally Forward Left");
        
        /*digitalWrite(moveForwardLED,HIGH);
        digitalWrite(moveBackwardLED,LOW);
        digitalWrite(turnLeftLED,HIGH);
        digitalWrite(turnRightLED,LOW);*/
        
        moveNorthWest();
        
        radio.stopListening();
        returnMessage=51;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=52;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 60) { //else if the transmission returns a value of 60
      currentDistanceCenter=determineCurrentDistance(rangeTriggerPinCenter,rangeEchoPinCenter); //determine the distance to the nearest object in the center
      currentDistanceRight=determineCurrentDistance(rangeTriggerPinRight,rangeEchoPinRight); //determine the distance to the nearest object to the right
      if(currentDistanceCenter>15 && currentDistanceRight>15) {
        Serial.println("Moving Diagonally Forward Right");
        
        /*digitalWrite(moveForwardLED,HIGH);
        digitalWrite(moveBackwardLED,LOW);
        digitalWrite(turnLeftLED,LOW);
        digitalWrite(turnRightLED,HIGH);*/
        
        moveNorthEast();
        
        radio.stopListening();
        returnMessage=61;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=62;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 70) { //else if the transmission returns a value of 70
      currentDistanceBack=determineCurrentDistance(rangeTriggerPinBack,rangeEchoPinBack); //determine the distance to the nearest object in the center
      currentDistanceLeft=determineCurrentDistance(rangeTriggerPinLeft,rangeEchoPinLeft); //determine the distance to the nearest object to the right
      if(currentDistanceBack>15 && currentDistanceLeft>15) {
        Serial.println("Moving Diagonnaly Backward Left");
        
        /*digitalWrite(moveForwardLED,LOW);
        digitalWrite(moveBackwardLED,HIGH);
        digitalWrite(turnLeftLED,HIGH);
        digitalWrite(turnRightLED,LOW);*/
        
        moveSouthWest();
        
        radio.stopListening();
        returnMessage=71;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=72;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 80) { //else if the transmission returns a value of 80
      currentDistanceBack=determineCurrentDistance(rangeTriggerPinBack,rangeEchoPinBack); //determine the distance to the nearest object in the center
      currentDistanceRight=determineCurrentDistance(rangeTriggerPinRight,rangeEchoPinRight); //determine the distance to the nearest object to the right
      if(currentDistanceBack>15 && currentDistanceRight>15) {
        Serial.println("Moving Diagonally Backward Right");
        
        /*digitalWrite(moveForwardLED,LOW);
        digitalWrite(moveBackwardLED,HIGH);
        digitalWrite(turnLeftLED,LOW);
        digitalWrite(turnRightLED,HIGH);*/
        
        moveSouthEast();
        
        radio.stopListening();
        returnMessage=81;
        radio.write(&returnMessage,1);
        radio.startListening();
      } else {
        Serial.println("HELP");
        
        stopMoving();
        
        radio.stopListening();
        returnMessage=82;
        radio.write(&returnMessage,1);
        radio.startListening();
      }
    } else if (message == 0) { //else if the transmission returns a value of 0
      Serial.println("Not Moving ");
      
      /*digitalWrite(moveForwardLED,LOW);
      digitalWrite(moveBackwardLED,LOW);
      digitalWrite(turnLeftLED,LOW);
      digitalWrite(turnRightLED,LOW);*/
      
      stopMoving();
      
      radio.stopListening();
      returnMessage=1;
      radio.write(&returnMessage,1);
      radio.startListening();
    }
  }
}

//method that determines the current distance to the nearest object
long determineCurrentDistance(int rangeTriggerPin, int rangeEchoPin) {
  digitalWrite(rangeTriggerPin, LOW); //first write the Trigger as low to create a clean signal
  delayMicroseconds(2);
  digitalWrite(rangeTriggerPin, HIGH); //release a PING from the Trigger pin
  delayMicroseconds(5);
  digitalWrite(rangeTriggerPin, LOW); //rewrite the Trigger to low after 5 microseconds to end the PING
  
  long duration=pulseIn(rangeEchoPin, HIGH); //use the Echo pin to read the Echo off of the PING
  
  return timeToCentimeters(duration); //calls the method that converts the time based Echo read to a distance
}

//method that converts the time based Echo read to a distance
long timeToCentimeters(long time) {
  //The speed of sound is 29 microseconds/centimeter
  //The Echo reads the time it takes for the ping to reach an object and come back so we consider half the time.
  return time / 29 / 2;
}

//method that initiates the Servos
void initServos() {
  leftServo.attach(7); //initiates association between left Servo motor and pin 9
  rightServo.attach(6); //initiates association between right Servo motor and pin 6
}

//method that moves the bot forward
void moveForward() {
  leftServo.writeMicroseconds(1700);
  rightServo.writeMicroseconds(1300);
}

//method that turns the bot left
void moveLeft() {
  leftServo.writeMicroseconds(1520);
  rightServo.writeMicroseconds(1300);
}

//method that turns the bot right
void moveRight() {
  leftServo.writeMicroseconds(1700);
  rightServo.writeMicroseconds(1520);
}

//method that moves bot NorthWest
void moveNorthWest() {
  leftServo.writeMicroseconds(1580);
  rightServo.writeMicroseconds(1300);
}

//method that moves bot NorthEast
void moveNorthEast() {
  leftServo.writeMicroseconds(1700);
  rightServo.writeMicroseconds(1480);
}

//method that moves bot SouthWest
void moveSouthWest() {
  leftServo.writeMicroseconds(1480);
  rightServo.writeMicroseconds(1700);
}

//method that moves bot SouthEast
void moveSouthEast() {
  leftServo.writeMicroseconds(1300);
  rightServo.writeMicroseconds(1560);
}

//method that moves bot backward
void moveBackward() {
  leftServo.writeMicroseconds(1300);
  rightServo.writeMicroseconds(1700);
}

//method that stops bot 
void stopMoving() {
  leftServo.writeMicroseconds(1520);
  rightServo.writeMicroseconds(1520);
}
