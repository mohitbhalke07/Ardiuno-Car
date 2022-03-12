#include <SoftwareSerial.h>
SoftwareSerial BT_Serial(0,1); // RX, TX


#define m1 9  //RIGHT Motor forword Pin   IN1
#define m2 8  //RIGHT Motor backword Pin  IN2
#define enableA 10 /*RHS MOTORS*/

#define m3 7  //LEFT Motor backword Pin  IN3
#define m4 6  //LEFT Motor forword Pin   IN4
#define enableB 5 /*LHS MOTORS*/

const int R_S = A2;                                    //Assign the IR sensor pins
const int L_S = A3; 

int data;

int Right_Value;
int Left_Value;

void setup() {
  // put your setup code here, to run once:
  pinMode(m1, OUTPUT);  
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);   
  pinMode(m4, OUTPUT);

  pinMode(enableA/*RHS MOTORS*/, OUTPUT);
  pinMode(enableB/*LHS MOTORS*/, OUTPUT);

  pinMode(R_S, INPUT);
  pinMode(L_S, INPUT);

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

  if(data == 'S')
  {
    digitalWrite(m1, LOW);   //RIGHT Motor  1 
    digitalWrite(m2, LOW);   //RIGHT Motor  2
    digitalWrite(m3, LOW);   //LEFT Motor   3
    digitalWrite(m4, LOW);   //LEFT Motor   4
    }
  else if(data == 'D')
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
