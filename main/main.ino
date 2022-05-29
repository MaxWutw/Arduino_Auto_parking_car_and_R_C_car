#include <SoftwareSerial.h> // 引用「軟體序列埠」程式庫
#include<Servo.h>
SoftwareSerial BT(2, 3); // 設定軟體序列埠(接收腳, 傳送腳)
Servo myservo;
char data; // 接收序列埠值的變數
const int In1 = 4;
const int In2 = 5;
const int In3 = 6;      
const int In4 = 7; 
const int trigPin = 12; //Trig Pin
const int echoPin = 11; //Echo Pin
int finish = 0, finish_back = 0;
long duration, cm;
unsigned long start;
bool park = true,servo = true,pd = true;//parking detect
void setup() {
  myservo.attach(9);
  myservo.write(0);
  Serial.begin(9600);
  BT.begin(9600); // connect BT,rate = 9600
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);   
  pinMode(trigPin, OUTPUT); // declared I/O
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
}

void loop(){
  if(BT.available()){
    data = BT.read();
    switch(data){
    case 'f' : //forward
      forward();
      break;
    case 'b' : //back
      backward();
      break;
    case 'l' : //left
      turnleft();
      break;
    case 'r' : //right
      turnright();
      break;
    case 's' : //stop
      stopping();
      break;
    case 'a': //parking
      park = true;
      autoparking();
      break;
    }
  }
}
void forward(){
  digitalWrite(In1,LOW);
  digitalWrite(In4,LOW);
  analogWrite(In2, 80);
  analogWrite(In3, 80);
}
void backward(){
  digitalWrite(In1,HIGH);
  digitalWrite(In4,HIGH);
  analogWrite(In2,170);
  analogWrite(In3,170);
}
void turnleft(){
  digitalWrite(In1,LOW);
  digitalWrite(In4,LOW);
  analogWrite(In2, 0);
  analogWrite(In3, 80);
}
void turnright(){
  digitalWrite(In1,LOW);
  digitalWrite(In4,LOW);
  analogWrite(In2, 80);
  analogWrite(In3, 0);
}
void stopping(){
  digitalWrite(In1,LOW);
  digitalWrite(In4,LOW);
  analogWrite(In2, 0);
  analogWrite(In3, 0);
}
int autoparking(){
  digitalWrite(In1,LOW);
  digitalWrite(In4,LOW); 
  analogWrite(In2, 50);
  analogWrite(In3, 42);
  while(park == true){
    detect();
    if(cm >= 16){
      start = millis();
      while(cm >= 16){
        detect();
      }
        finish = millis()-start;
        detect();
        if(finish < 1562){
          Serial.println(finish);
          myservo.write(100);
          digitalWrite(In1,HIGH);
          digitalWrite(In4,HIGH);
          analogWrite(In2,190);
          analogWrite(In3,190);
          delay(300);
          digitalWrite(In1,HIGH);
          digitalWrite(In4,LOW);
          analogWrite(In2, 70);
          analogWrite(In3, 50);
          delay(430);
          digitalWrite(In1,HIGH);
          digitalWrite(In4,HIGH);
          analogWrite(In2,175);
          analogWrite(In3,175);
          delay(200);
          digitalWrite(In1,LOW);
          digitalWrite(In4,LOW);
          analogWrite(In2, 0);
          analogWrite(In3, 0);
          myservo.write(0);
          park = false;
          return 0;
        }
        Serial.println(finish);
        myservo.write(100);
        digitalWrite(In1,HIGH);
        digitalWrite(In4,HIGH);
        analogWrite(In2,190);
        analogWrite(In3,190);
        delay(300);
        digitalWrite(In1,HIGH);
        digitalWrite(In4,LOW);
        analogWrite(In2, 85);
        analogWrite(In3, 65);
        delay(480);
        digitalWrite(In1,HIGH);
        digitalWrite(In4,HIGH);
        analogWrite(In2,170);
        analogWrite(In3,170);
        delay(450);
        detect();
        int k = 0;
        while(k != 5){
          digitalWrite(In1,LOW);//right
          digitalWrite(In4,LOW);
          analogWrite(In2, 120);
          analogWrite(In3, 50);
          delay(460);
          digitalWrite(In1,HIGH);//back
          digitalWrite(In4,HIGH);
          analogWrite(In2, 170);
          analogWrite(In3, 170);
          delay(450); 
          k++;   
        }     
        digitalWrite(In1,LOW);
        digitalWrite(In4,LOW);
        analogWrite(In2, 60);
        analogWrite(In3, 60);
        delay(500);
        digitalWrite(In1,LOW);
        digitalWrite(In4,LOW);
        analogWrite(In2, 0);
        analogWrite(In3, 0);
        myservo.write(0);
        park = false;
        return 0;
    }        
    else{ 
      Serial.print("Distance : ");  
      Serial.print(cm);
      Serial.print("cm");
      Serial.println();
      delay(250);
    }
  }
}
void detect(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); //give Trig HIGH,continue 10 microsecond
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT); //read echo electric 
  duration = pulseIn(echoPin, HIGH); // time of reciving HIGH time
  cm = (duration/2) / 29.1; //time to cm
}
