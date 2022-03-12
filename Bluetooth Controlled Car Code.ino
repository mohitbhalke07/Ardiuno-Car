#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(0,1); // RX, TX
#include<Servo.h>

Servo servoLook;
const int trig = A0;
const int echo = A1;
 
#define m1 9  //RIGHT Motor forword Pin   IN1
#define m2 8  //RIGHT Motor backword Pin  IN2
#define enableA 5 /*RHS MOTORS*/

#define m3 7  //LEFT Motor backword Pin  IN3
#define m4 6  //LEFT Motor forword Pin   IN4
#define enableB 10 /*LHS MOTORS*/ 

int data; //To Receive data from the Serial Port
int Speed = 255;  
int pos;
int distance;
int duration;

void setup()
{ // put your setup code here, to run once

  pinMode(m1, OUTPUT);  
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);   
  pinMode(m4, OUTPUT);
 
  pinMode(enableA/*RHS MOTOR*/, OUTPUT);
  pinMode(enableB/*LHS MOTOR*/, OUTPUT); 

  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);

  servoLook.attach(4);
  
  servoLook.write(95);
  for(pos = 95; pos <= 180; pos += 2)
    {
      servoLook.write(pos);
      delay(15);
    } 
    for(pos = 180; pos >= 0; pos-= 2)
    {
      servoLook.write(pos);
      delay(15);
    }
    for(pos = 0; pos<=95; pos += 2) 
    {
      servoLook.write(pos);
      delay(15);
    } 

  Serial.begin(9600);
  BT_Serial.begin(9600); 
  delay(200);

}

void loop()
{  

  if(BT_Serial.available() > 0)
  { 
    data = BT_Serial.read();      
  }
  
  analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

  distance = getDistance();
  Serial.println('Forward Distance: ');
  Serial.print(distance);
  
  if(data == 'F' && distance > 50)
     {forword(); } 
     else if(data == 'F')
      {Stop();}
  else if(data == 'B')
     {backword();} 
  else if(data == 'L' && distance > 40)
     {turnLeft();}  
     else if(data == 'L')
      {Stop();}
  else if(data == 'R' && distance > 35)
     {turnRight();} 
     else if(data == 'R')
      {Stop();} 
  else if(data == 'S')
     {Stop();}   
    else if(data == 'A')  //SERVO FRONT
     {ServoFront(); } 
    else if(data == 'E')  //SERVO RIGHT
     {ServoRight();}  
    else if(data == 'C')  //SERVO LEFT
     {ServoLeft();}
  delay(20);
}

int getDistance()
{
  digitalWrite(trig , LOW);
  delayMicroseconds(10);
  digitalWrite(trig , HIGH);
  delayMicroseconds(25);
  digitalWrite(trig , LOW);
  duration = pulseIn(echo , HIGH);
  distance = duration*0.034/2;
  return distance;
  }

int ServoFront()
{
  servoLook.write(95);
  delay(20);
}

int ServoRight()
{
  servoLook.write(180);
  delay(50);
  distance = getDistance();
  Serial.println('Right Distance: ');
  Serial.print(distance);
}

int ServoLeft()
{
    servoLook.write(0);
    delay(50);
    distance = getDistance();
    Serial.println('Left Distance: ');
    Serial.print(distance);
}

void forword()
{  
  
  digitalWrite(m1, HIGH);  //RIGHT Motor 1 
  digitalWrite(m2, LOW);   //RIGHT Motor 2
  digitalWrite(m3, HIGH);  //LEFT Motor  3
  digitalWrite(m4, LOW);   //LEFT Motor  4

}

void backword()
{ 
  
  digitalWrite(m1, LOW);  //Right Motor forword Pin 
  digitalWrite(m2, HIGH); //Right Motor backword Pin 
  digitalWrite(m3, LOW); //Left Motor backword Pin 
  digitalWrite(m4, HIGH);  //Left Motor forword Pin 

}

void turnRight()
{ 
  
  digitalWrite(m1, LOW);     //RIGHT Motor 1 
  digitalWrite(m2, HIGH);    //RIGHT Motor 2
  digitalWrite(m3, HIGH);    //LEFT Motor  3
  digitalWrite(m4, LOW);     //LEFT Motor  4

}

void turnLeft()
{
  
  digitalWrite(m1, HIGH);   //RIGHT Motor 1 
  digitalWrite(m2, LOW);    //RIGHT Motor 2
  digitalWrite(m3, LOW);    //LEFT Motor  3
  digitalWrite(m4, HIGH);   //LEFT Motor  4
 
}

void Stop()
{
  
  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 

}
