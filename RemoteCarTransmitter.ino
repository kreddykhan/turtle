/**
 * Karishma Reddy Khan
 * Can be loaded on to an Arduino Uno to create a remote control RF transmitter
 **/

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

int CE_Pin= 9; //associate the CE Pin with pin 9
int CS_Pin=10; //associate the CS Pin with pin 10
RF24 radio (CE_Pin, CS_Pin); //associate the CE and CS Pins with the radio
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; //establish the two addresses to be used with the radio

int message; //the initial transmission message
int returnMessage; //the returned transmission message

int moveForwardButton = 2; //associate the move forward button with pin 2
int moveBackwardButton = 3; //associate the move backward button with pin 3
int turnLeftButton = 4; //associate the turn left button with pin 4
int turnRightButton = 5; //associate the turn right button with pin 5

int forwardLED=15; //associate the LED which indicates an obstacle in front of the bot with pin 15/A1
int backwardLED=18; //associate the LED which indicates an obstable behind the bot with pin 18/A4
int leftLED=17; //associate the LED which indicates an obstacle to the left of the bot with pin 17/A3
int rightLED=16; //associate the LED which indicates an obstacle to the right of the bot with pin 16/A4

void setup() {
  Serial.begin(57600); //begin the serial channel 57600
  
  radio.begin(); //initialize the radio
  radio.openWritingPipe(pipes[0]); //associate the writing pipe with address 1
  radio.openReadingPipe(1,pipes[1]); //associate the reading pipe with address 2
  
  //establish the buttons as INPUTS
  pinMode(moveForwardButton,INPUT);
  pinMode(moveBackwardButton,INPUT);
  pinMode(turnLeftButton,INPUT);
  pinMode(turnRightButton,INPUT);
  
  //establish the LEDs as OUTPUTS
  pinMode(forwardLED,OUTPUT);
  pinMode(backwardLED,OUTPUT);
  pinMode(leftLED,OUTPUT);
  pinMode(rightLED,OUTPUT);
}

void loop() {
  //digital write all the LEDs low at the beginning of the loop to overwrite any previous obstacles
  digitalWrite(forwardLED,LOW);
  digitalWrite(backwardLED,LOW);
  digitalWrite(leftLED,LOW);
  digitalWrite(rightLED,LOW);
  
  //perform and store a digital read of all the buttons
  int moveForward=digitalRead(moveForwardButton);
  int moveBackward=digitalRead(moveBackwardButton);
  int turnLeft=digitalRead(turnLeftButton);
  int turnRight=digitalRead(turnRightButton);
  
  //if the only button that registers a LOW state (which is registered when a button is pushed down) is the forward button
  if(moveForward == LOW && moveBackward == HIGH && turnLeft == HIGH && turnRight == HIGH) {
    message=10;
    radio.write(&message, 1); //transmit the value 10 across the radio
    
    radio.startListening(); //start listening for a return transmission
    while ( ! radio.available()) {;} //while the return transmission is not available, wait
    radio.read(&returnMessage,1); //read the return transmission
    Serial.println(returnMessage);
    if(returnMessage==12) { //if the return transmission is 12 (the value returned when there is an obstacle
      digitalWrite(forwardLED, HIGH); //digital write the forward LED HIGH
    }
    radio.stopListening(); //stop listening for a return transmission
  } else if (moveBackward == LOW && moveForward == HIGH && turnLeft == HIGH && turnRight == HIGH) { //if the only button that registers a LOW state is the backward button
    message=20;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==22) {
      digitalWrite(backwardLED, HIGH);
    }
    radio.stopListening();
  } else if (turnLeft == LOW && moveForward == HIGH && moveBackward == HIGH && turnRight == HIGH) { //if the only button that registers a LOW state is the turn left button
    message=30;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==32) {
      digitalWrite(leftLED, HIGH);
    }
    radio.stopListening();
  } else if (turnRight == LOW && moveForward == HIGH && moveBackward == HIGH && turnLeft == HIGH) { //if the only button that registers a LOW state is the turn right button
    message=40;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==42) {
      digitalWrite(rightLED, HIGH);
    }
    radio.stopListening();
  } else if (moveForward == LOW && turnLeft == LOW && moveBackward == HIGH && turnRight == HIGH) { //if the forward and turn left buttons both register LOW states
    message=50;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==52) {
      digitalWrite(forwardLED, HIGH);
      digitalWrite(leftLED, HIGH);
    }
    radio.stopListening();
  } else if (moveForward == LOW && turnRight == LOW && moveBackward == HIGH && turnLeft == HIGH) { //if the forward and turn right buttons both register LOW states
    message=60;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==62) {
      digitalWrite(forwardLED, HIGH);
      digitalWrite(rightLED, HIGH);
    }
    radio.stopListening();
  } else if (moveBackward == LOW && turnLeft == LOW && moveForward == HIGH && turnRight == HIGH) { //if the backward and turn left buttons both register LOW states
    message=70;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==72) {
      digitalWrite(backwardLED, HIGH);
      digitalWrite(leftLED, HIGH);
    }
    radio.stopListening();
  } else if (moveBackward == LOW && turnRight == LOW && moveForward == HIGH && turnLeft == HIGH) { //if the backward and turn right buttons both register LOW states
    message=80;
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    if(returnMessage==82) {
      digitalWrite(backwardLED, HIGH);
      digitalWrite(rightLED, HIGH);
    }
    radio.stopListening();
  } else if (moveForward == LOW && moveBackward == LOW && turnLeft == HIGH && turnRight == HIGH) { //if the forward and backward buttons both register LOW states
    message=0; //transmit a value of 0 which results in no motion
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    radio.stopListening();
  } else if (turnLeft == LOW && turnRight == LOW && moveForward == HIGH && moveBackward == HIGH) { //if the turn left and turn right buttons both register LOW states
    message=0; //transmit a value of 0 which results in no motion
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    radio.stopListening();
  } else if (moveForward == HIGH && moveBackward == HIGH && turnLeft == HIGH && turnRight == HIGH) { //if none of the buttons register a LOW state
    message=0; //transmit a value of 0 which results in no motion
    radio.write(&message, 1);
    
    radio.startListening();
    while ( ! radio.available()) {;}
    radio.read(&returnMessage,1);
    Serial.println(returnMessage);
    radio.stopListening();
  }
}
