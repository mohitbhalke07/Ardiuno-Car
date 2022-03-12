#include<Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial BT_Serial(0,1); // RX,TX

#define m1 9  //RIGHT Motor forword Pin   IN1
#define m2 8  //RIGHT Motor backword Pin  IN2
#define enableA 5 /*RHS MOTORS*/

#define m3 7  //LEFT Motor backword Pin  IN3
#define m4 6  //LEFT Motor forword Pin   IN4
#define enableB 10 /*LHS MOTORS*/

const int trig = A0;
const int echo = A1;

Servo servoLook;

long duration;
int pos = 0;
int maxdistance = 100;
int stopdistance = 25;
int data;
int distance;
int distance_F;
int distance_R;
int distance_L;

void setup() {
  // put your setup code here, to run once:
  pinMode(m1, OUTPUT);  
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);   
  pinMode(m4, OUTPUT);

  pinMode(enableA/*RHS MOTORS*/, OUTPUT);
  pinMode(enableB/*LHS MOTORS*/, OUTPUT);

  servoLook.attach(4);

  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);

  servoLook.write(95);
  for(pos = 95; pos <= 180; pos += 2)
    {
      servoLook.write(pos);
      delay(20);
    } 
    for(pos = 180; pos >= 0; pos-= 2)
    {
      servoLook.write(pos);
      delay(20);
    }
    for(pos = 0; pos<=95; pos += 2) 
    {
      servoLook.write(pos);
      delay(20);
    } 
    
   Serial.begin(9600);
   BT_Serial.begin(9600);
   delay(200);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(BT_Serial.available() > 0)
  { 
    data = BT_Serial.read();      
  }  
  if(data == 5)
  {
    Stop();
    }
  else if(data == 9)
  { 
  delay(500);
  distance_F = getDistance();
  Serial.print("Distance : ");
  Serial.print(distance_F);

  if(distance_F > stopdistance)
  {
    Forward();
    }
   else if (distance_F <= stopdistance)
   {
    checkDirection();
    }
    
   if(distance_R > distance_L)
   {
    turnRight(400);
    }
   else if(distance_L > distance_R)
   {
    turnLeft(400);
    }
   else if(distance_R <= stopdistance && distance_L <=  stopdistance)
   {
    turnAround_Right(700);
    }
  }
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

int checkDirection()
{
  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 

  servoLook.write(0);
   delay(500);
   distance_R = getDistance();
   delay(250);
   Serial.print("Distance_R : ");
   Serial.print(distance_R);
   
   servoLook.write(180);
   delay(700);
   distance_L = getDistance();
   delay(250);
   Serial.print("Distance_L : ");
   Serial.print(distance_L);

   servoLook.write(95);
   delay(500);
  }
  
void Forward()
{
  digitalWrite(m1, HIGH);  //RIGHT Motor 1 
  digitalWrite(m2, LOW);   //RIGHT Motor 2
  digitalWrite(m3, HIGH);  //LEFT Motor  3
  digitalWrite(m4, LOW);   //LEFT Motor  4

  analogWrite(enableA, 90); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 90); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
  }

void Stop()
{
  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 

  }
  
void turnRight(int duration1)
{
  analogWrite(enableA, 130); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 130); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
  digitalWrite(m1, LOW);     //RIGHT Motor 1 
  digitalWrite(m2, HIGH);    //RIGHT Motor 2
  digitalWrite(m3, HIGH);    //LEFT Motor  3
  digitalWrite(m4, LOW);     //LEFT Motor  4

  delay(duration1);

  analogWrite(enableA, 90); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 90); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 
  
  }
  
void turnLeft(int duration1)
{
  analogWrite(enableA, 130);                    // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 130);        // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

  digitalWrite(m1, HIGH);   //RIGHT Motor 1 
  digitalWrite(m2, LOW);    //RIGHT Motor 2
  digitalWrite(m3, LOW);    //LEFT Motor  3
  digitalWrite(m4, HIGH);   //LEFT Motor  4
  
  delay(duration1);

  analogWrite(enableA, 90);                     // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 90);         // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 

  }
void turnAround_Right(int duration1)
{
  analogWrite(enableA, 130); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 130); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
  digitalWrite(m1, LOW);     //RIGHT Motor 1 
  digitalWrite(m2, HIGH);    //RIGHT Motor 2
  digitalWrite(m3, HIGH);    //LEFT Motor  3
  digitalWrite(m4, LOW);     //LEFT Motor  4

  delay(duration1);

  analogWrite(enableA, 90); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
  analogWrite(enableB, 90); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

  digitalWrite(m1, LOW);   //RIGHT Motor  1 
  digitalWrite(m2, LOW);   //RIGHT Motor  2
  digitalWrite(m3, LOW);   //LEFT Motor   3
  digitalWrite(m4, LOW);   //LEFT Motor   4 
  }
