#include "SoftwareSerial.h"
SoftwareSerial serial_connection(0  , 1);
#define BUFFER_SIZE 64    //This will prevent buffer overruns.
char inData[BUFFER_SIZE]; //This is a character buffer where the data sent by the python script will go.
char inChar=-1;           //Initialie the first character as nothing
int count=0;              //This is the number of lines sent in from the python script
int i=0;                  

int SA = 9;               //Pins for motors
int DA = 2;
int SB = 5;
int DB = 4;

void setup()
{
  pinMode(SA, OUTPUT);
  pinMode(SB, OUTPUT);
  pinMode(DA, OUTPUT);
  pinMode(DB, OUTPUT);

  Serial.begin(9600);
  serial_connection.begin(9600);
  serial_connection.println("Ready!!!"); //Send something to just start comms. This will never be seen.
  Serial.println("Started");             //Tell the serial monitor that the sketch has started.
}

void stop() {
  digitalWrite(DA, HIGH);
  digitalWrite(DB, HIGH);
  analogWrite(SA, 0);
  analogWrite(SB, 0);
}

void forward() {
  digitalWrite(DA, HIGH);
  digitalWrite(DB, HIGH);
  analogWrite(SA, 150);
  analogWrite(SB, 150);
}

void backward() {
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);
  analogWrite(SA, 150);
  analogWrite(SB, 150);
}

void left() {
  digitalWrite(DA, LOW);
  digitalWrite(DB, HIGH);
  analogWrite(SA, 0);
  analogWrite(SB, 150);
}

void right() {
  digitalWrite(DA, HIGH);
  digitalWrite(DB, LOW);
  analogWrite(SA, 150);
  analogWrite(SB, 0);
}

void loop()
{
  //This will prevent bufferoverrun errors
  byte byte_count=serial_connection.available();
  if(byte_count)
  {
    Serial.println("Incoming Data"); //Signal to the monitor that something is happening
    int first_bytes=byte_count;      //initialize the number of bytes that we might handle. 
    int remaining_bytes=0;           //Initialize the bytes that we may have to burn off to prevent a buffer overrun
    if(first_bytes>=BUFFER_SIZE-1)   //If the incoming byte count is more than our buffer...
    {                                //Reduce the bytes that we plan on handling to below the buffer size
      remaining_bytes=byte_count-(BUFFER_SIZE-1);
    }
    for(i=0;i<first_bytes;i++)       //Handle the number of incoming bytes
    {
      inChar=serial_connection.read();
      inData[i]=inChar; //Read one byte and put it into a character string(array)
    }
    inData[i]='\0'; //This ends the character array with a null character. This signals the end of a string
    if(String(inData)=="0")
    {
      Serial.println("** Stop ********");
      stop();
    }
    else if(String(inData)=="1")
    {
      Serial.println("** Forward *****");
      forward();
    }
    else if(String(inData)=="2")
    {
      Serial.println("** Backward ****");
      backward();
    }
    else if(String(inData)=="3")
    {
      Serial.println("** Left ********");
      left();
    }
    else if(String(inData)=="4")
    {
      Serial.println("** Right *******");
      right();
    }
    for(i=0;i<remaining_bytes;i++) //This burns off any remaining bytes that the buffer can't handle.
    {
      inChar=serial_connection.read();
    }
    count++;//Increment the line counter
  }
  delay(100);//Pause for a moment 
}
