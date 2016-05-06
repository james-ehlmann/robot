/*
 * Richard LaFramboise
 * 
 * This is a simple program designed to take input from a Raspberry Pi and act accordingly
 * 
 * It is important to note that the kill command does not close everything out, but it 
 * simply puts the Arduino in an infinite loop. Getting everything running again requires a
 * manual restart of both systems. void loop() is continously called, and I can't stop it.
 */

#include <Servo.h>

const int ledPin = 13;

// define servos
Servo left;
Servo right;
Servo paint1;
Servo paint2;
Servo paint3;

// define amount for wheels to turn 
int quarter = 325;
int full = 1240;
int half = 700; // currently a rough estimate and has not been tested

void setup(){
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  // attach servos to ports
  left.attach(8); 
  right.attach(9); 
  paint1.attach(10); 
  paint2.attach(11); 
  paint3.attach(12); 
}

void loop(){
  
  // just a short example of an if statement inside the loop
  /*if (Serial.available())  {
     blink(Serial.read() - '0');  // convert the character '1'-'9' to decimal 1-9
  }*/

  char input = Serial.read();

  // check for forward command
  if(input == 'F')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    GoForward();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for backward command
  else if(input == 'B')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    GoBackward();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for right turn command
  else if(input == 'R')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    TurnRight();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for left turn command
  else if(input == 'L')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    TurnLeft();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for command to shoot green paint
  else if(input == 'A')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    SprayPaint1();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for command to shoot red paint
  else if(input == 'E')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    SprayPaint2();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for command to shoot blue paint
  else if(input == 'U')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    SprayPaint3();

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // check for the kill command
  else if(input == 'K')
  {
    // send a 'G' saying that we got it
    Serial.write('G');

    // execute the command corresponding to the input letter
    left.writeMicroseconds(1500);
    right.writeMicroseconds(1500);
    while(1){} // turns out I can't use break. Thanks Arduino.

    // send a 'D' saying that we are done running the command
    Serial.write('D');
  }

  // if there are no commands, just sit there, but continue listening for commands
  else
  {
    left.writeMicroseconds(1500);
    right.writeMicroseconds(1500);
  }
  
  delay(500);
}

// method used to test communication between the pi and the arduino
/*void blink(int numberOfTimes){
  for (int i = 0; i < numberOfTimes; i++)  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}*/

//////////// MOVEMENT COMMANDS //////////////////////

void GoBackward()
{
  left.writeMicroseconds(1700); 
  right.writeMicroseconds(1300);
  delay(full);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void GoForward()
{
  left.writeMicroseconds(1300); 
  right.writeMicroseconds(1700);
  delay(full);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void TurnLeft()
{
  left.writeMicroseconds(1700); 
  right.writeMicroseconds(1700);
  delay(5000);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void TurnRight()
{
  left.writeMicroseconds(1300); 
  right.writeMicroseconds(1300);
  delay(5000);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint1()
{
  paint1.writeMicroseconds(1300); 
  delay(half);
  paint1.writeMicroseconds(1500);
  delay(1000);

  paint1.writeMicroseconds(1700); 
  delay(half);
  paint1.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint2()
{
  paint2.writeMicroseconds(1300); 
  delay(half);
  paint2.writeMicroseconds(1500);
  delay(1000);

  paint2.writeMicroseconds(1700); 
  delay(half);
  paint2.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint3()
{
  paint3.writeMicroseconds(1300); 
  delay(half);
  paint3.writeMicroseconds(1500);
  delay(1000);

  paint3.writeMicroseconds(1700); 
  delay(half);
  paint3.writeMicroseconds(1500);
  delay(1000);
}
