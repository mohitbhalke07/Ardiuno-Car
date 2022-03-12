#include<Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(0,1); // RX, TX

#define m1 9  //RIGHT Motor forword Pin   IN1
#define m2 8  //RIGHT Motor backword Pin  IN2
#define enableA 10 /*RHS MOTORS*/

#define m3 7  //LEFT Motor backword Pin  IN3
#define m4 6  //LEFT Motor forword Pin   IN4
#define enableB 5 /*LHS MOTORS*/

const int trig = A0;
const int echo = A1;

const int R_S = A2;   //IR SENSOR CONNECTION
const int L_S = A3;
int Right_Value;
int Left_Value;

Servo servoLook;

long duration;
int pos = 0;
int distance;
int distance_F;
int distance_R;
int distance_L;
int Speed = 255;
int data = 0;
int Mode;

void setup() {
  // put your setup code here, to run once:
  pinMode(m1, OUTPUT);  
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);   
  pinMode(m4, OUTPUT);
 
  pinMode(enableA/*RHS MOTOR*/, OUTPUT);
  pinMode(enableB/*LHS MOTOR*/, OUTPUT); 

  pinMode(trig , OUTPUT);
  pinMode(echo , INPUT);

  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

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

void loop() {
  // put your main code here, to run repeatedly:

  if(BT_Serial.available() > 0)
  { 
    data = BT_Serial.read();      
  }

  if(data == 0 ) //MANUAL MODE
     {Mode=1;}   
  else if(data =='O' ) //OBSTRACLE MODE
     {Mode=2;} 
  else if(data =='D' ) //LINE MODE
     {Mode=3;}

  if(Mode==1)
  {
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


    else if(Mode==2)
    {
      if(data=='S')
      {Stop();}
      else
      {
      distance_F = getDistance();
      Serial.print("Distance_F : ");
      Serial.print(distance_F);
    
      if(distance_F > 55)
        { //FORWARD
          digitalWrite(m1, HIGH);  //RIGHT Motor 1 
          digitalWrite(m2, LOW);   //RIGHT Motor 2
          digitalWrite(m3, HIGH);  //LEFT Motor  3
          digitalWrite(m4, LOW);   //LEFT Motor  4
        
          analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
          analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
          }
      else
        {CheckDirection();}
      delay(50);
      }
      }

  else if(Mode==3)
    {
      if(data=='S')
      {Stop();}
      else
      {
        Right_Value = digitalRead(R_S);
  Serial.println("RIGHT VALUE: ");
  Serial.print(Right_Value);
 
  Left_Value  = digitalRead(L_S);
  Serial.println("LEFT VALUE: ");
  Serial.print(Left_Value);

  if(Right_Value == 0/*WHITE*/ && Left_Value == 0) //FORWARD
  {
    digitalWrite(m1, HIGH);  //RIGHT Motor 1 
    digitalWrite(m2, LOW);   //RIGHT Motor 2
    digitalWrite(m3, HIGH);  //LEFT Motor  3
    digitalWrite(m4, LOW);   //LEFT Motor  4
  
    analogWrite(enableA, 90);                 // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, 90);                 // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

    }  

  if(Right_Value == 1/*BLACK*/ && Left_Value == 0) //TURN RIGHT
  {
    digitalWrite(m1, LOW);     //RIGHT Motor 1 
    digitalWrite(m2, HIGH);    //RIGHT Motor 2
    digitalWrite(m3, HIGH);    //LEFT Motor  3
    digitalWrite(m4, LOW);     //LEFT Motor  4

    analogWrite(enableA, 255);                 // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, 255);                 // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

    } 

  if(Right_Value == 0 && Left_Value == 1) //TURN LEFT
  {
    digitalWrite(m1, HIGH);   //RIGHT Motor 1 
    digitalWrite(m2, LOW);    //RIGHT Motor 2
    digitalWrite(m3, LOW);    //LEFT Motor  3
    digitalWrite(m4, HIGH);   //LEFT Motor  4
  
    analogWrite(enableA, 255);                 // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, 255);                 // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

    } 

  if(Right_Value == 1 && Left_Value == 1)
  {
    digitalWrite(m1, LOW);   //RIGHT Motor  1 
    digitalWrite(m2, LOW);   //RIGHT Motor  2
    digitalWrite(m3, LOW);   //LEFT Motor   3
    digitalWrite(m4, LOW);   //LEFT Motor   4
  
    analogWrite(enableA, 0);                 // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, 0);                 // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 

    }
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

int CheckDirection()  //OBSTRACLE 
{
   Stop();
   servoLook.write(0);  //TURN RIGHT
   delay(500);
   distance_R = getDistance();
   delay(250);
   Serial.print("Distance_R : ");
   Serial.print(distance_R);
   
   servoLook.write(180);  //TURN LEFT
   delay(700);
   distance_L = getDistance();
   delay(250);
   Serial.print("Distance_L : ");
   Serial.print(distance_L);

   servoLook.write(95);  //FRONT
   delay(500);

  if(distance_R > distance_L)
   { //TURN RIGHT
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
    
    digitalWrite(m1, LOW);     //RIGHT Motor 1 
    digitalWrite(m2, HIGH);    //RIGHT Motor 2
    digitalWrite(m3, HIGH);    //LEFT Motor  3
    digitalWrite(m4, LOW);     //LEFT Motor  4
  
    delay(250);
    
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
    Stop();
    }
   else if(distance_L > distance_R)
   { //TURN LEFT
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
    digitalWrite(m1, HIGH);   //RIGHT Motor 1 
    digitalWrite(m2, LOW);    //RIGHT Motor 2
    digitalWrite(m3, LOW);    //LEFT Motor  3
    digitalWrite(m4, HIGH);   //LEFT Motor  4
  
    delay(250);
  
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
    Stop();
    }
   else
   { //TURN AROUND RIGHT
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
    
    digitalWrite(m1, LOW);     //RIGHT Motor 1 
    digitalWrite(m2, HIGH);    //RIGHT Motor 2
    digitalWrite(m3, HIGH);    //LEFT Motor  3
    digitalWrite(m4, LOW);     //LEFT Motor  4
  
    delay(500);
    
    analogWrite(enableA, Speed); // Write The Duty Cycle 0 to 255 Enable Pin A for 2 RHS Motor Speed 
    analogWrite(enableB, Speed); // Write The Duty Cycle 0 to 255 Enable Pin B for 2 LHS Motor Speed 
  
    Stop();
    }
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
