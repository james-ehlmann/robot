/*
 * Richard LaFramboise
 * 
 * This is a simple program designed to take input from a Raspberry Pi and act accordingly
 * 
 * Also, this may be a helpful link:
 * http://www.seeedstudio.com/recipe/166-basic-pi-lt-gt-arduino-communication-over-usb.html
 */


#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>

///////// INITIALIZATION ///////////////

// define servos
Servo left;
Servo right;
Servo paint1;
Servo paint2;
Servo paint3;

#define SLAVE_ADDRESS 0x04
int input = 0;
int state = 0;

// define serial ports
// software serial : TX = digital pin 4, RX = digital pin 5
//SoftwareSerial port(4, 5); // try and use the default RX and TX ports on the board first

// define amount for wheels to turn 
int quarter = 325;
int full = 1240;
int half = 700; // currently a rough estimate and has not been tested

// create a string to hold any input sent by the pi
//String input = "";
// create a simple boolean value to let us know if the input string is complete
//boolean done = false;


///////// SETUP //////////////////
 
void setup() 
{ 
  // start hardware serial port
  Serial.begin(9600);
  // start the interface 
  //port.begin(9600);

  // reserve proper amount of space for input string 
  //input.reserve(256);

  //pinMode(13, OUTPUT);

  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  // attach servos to ports
  left.attach(8); 
  right.attach(9); 
  paint1.attach(10); 
  paint2.attach(11); 
  paint3.attach(12); 
} 


///////// PRIMARY LOOP /////////////////////


void loop() 
{ 
  /*
  // delay of 325 rotates the servo by 90 degrees
  // delay of 1240 rotates the servo by ~360

  // print the string when a newline arrives:
  if (stringComplete) 
  {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }  */

  delay(500);

}


//////// END OF LOOP /////////////////////

/*void serialEvent() 
{
  while (Serial.available()) 
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    input += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
      stringComplete = true;
    }
  }
}*/


// callback for received data
void receiveData(int byteCount){

    while(Wire.available())
    {
        // check to see if anything has been sent from the pi
        input = Wire.read();

        // check if we need to go forward
        if (input == 'F')
        {
            // set the input to G and send it to the pi to let it know we got the message
            int output = 'G';
            sendData();

            delay(1000);

            // execute
            GoForward();

            // send the signal to the pi that we are done 
            output = 'D';
            sendData();

            delay(1000);
        }
    }
}

// callback for sending data
void sendData()
{
    Wire.write(input);
}


//////////// MOVEMENT COMMANDS //////////////////////

void GoForward()
{
  left.writeMicroseconds(1700); 
  right.writeMicroseconds(1300);
  delay(full);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void GoBackward()
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
  delay(4000);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void TurnRight()
{
  left.writeMicroseconds(1300); 
  right.writeMicroseconds(1300);
  delay(4000);
  
  left.writeMicroseconds(1500);
  right.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint1()
{
  paint1.writeMicroseconds(1700); 
  delay(full);
  paint1.writeMicroseconds(1500);
  delay(1000);

  paint1.writeMicroseconds(1300); 
  delay(full);
  paint1.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint2()
{
  paint2.writeMicroseconds(1700); 
  delay(full);
  paint2.writeMicroseconds(1500);
  delay(1000);

  paint2.writeMicroseconds(1300); 
  delay(full);
  paint2.writeMicroseconds(1500);
  delay(1000);
}

void SprayPaint3()
{
  paint3.writeMicroseconds(1700); 
  delay(full);
  paint3.writeMicroseconds(1500);
  delay(1000);

  paint3.writeMicroseconds(1300); 
  delay(full);
  paint3.writeMicroseconds(1500);
  delay(1000);
}


/////// END /////////////////////////////



