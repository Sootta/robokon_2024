#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

Servo myServo;
const int SERVO_PIN = 4;
bool servo_dir = true;

const int RDIR_PIN = 18;
const int RPWM_PIN = 0;
const int LDIR_PIN = 19;
const int LPWM_PIN = 2;
bool moter_dir = true;

void setup() {
  Serial.begin(9600);
 
  myServo.attach(SERVO_PIN);

  pinMode(RDIR_PIN, OUTPUT);
  pinMode(LDIR_PIN, OUTPUT);

  ledcSetup(6, 4000, 8);
  ledcSetup(7, 4000, 8);
  ledcAttachPin(RPWM_PIN, 6);
  ledcAttachPin(LPWM_PIN, 7);
  
  PS4.begin("08:b6:1F:ed:54:40");
  Serial.println("Ready!");
  myServo.write(90);
}

void loop() {
  if(!PS4.isConnected()) {
    ledcWrite(6, 0);
    ledcWrite(7, 0);
    return;
  }

  int rspeed = PS4.R2Value();
  int lspeed = PS4.L2Value();
  
  if(PS4.Circle()){
    servo_dir = !servo_dir;
    myServo.write(servo_dir ? 90:10);
    delay(500);
  }

  if(PS4.Triangle()){
    moter_dir = !moter_dir;
    delay(500);
  }  

  if(abs(rspeed) > 30){
    ledcWrite(6, rspeed);
    digitalWrite(RDIR_PIN, moter_dir ? HIGH:LOW);
  }
  else{
    ledcWrite(6, 0);
  }
  if(abs(lspeed) > 30){
    ledcWrite(7, lspeed);
    digitalWrite(LDIR_PIN, moter_dir ? LOW:HIGH);
  }
  else{
    ledcWrite(7, 0);
  }


  delay(100);
}
