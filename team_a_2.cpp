#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <ESP32Encoder.h>

Servo myServo;
const int SERVO_PIN = 4;
bool servo_dir = true;

//跳ばすモーター２つ
const int THR_RIGHT_DIR_PIN = 18;
const int THR_RIGHT_PWM_PIN = 0;
const int THR_LEFT_DIR_PIN = 19;
const int THR_LEFT_PWM_PIN = 2;
bool moter_dir = true;

//車輪のモーター２つ
const int RIGHT_DIR_PIN;
const int RIGHT_PWM_PIN;
const int LEFT_DIR_PIN;
const int LEFT_PWM_PIN;

//ロリコン
ESP32Encoder encoder;
const int A_PIN;
const int B_PIN;
const int X_PIN;
int count = 0;
void handleIndexPulse() {
  encoder.clearCount();
}

void setup() {
  Serial.begin(9600);
 
  myServo.attach(SERVO_PIN);
  //車輪のモーター２つ
  pinMode(THR_RIGHT_DIR_PIN, OUTPUT);
  pinMode(THR_LEFT_DIR_PIN, OUTPUT);
  ledcSetup(4, 4000, 8);
  ledcSetup(5, 4000, 8);
  ledcAttachPin(RIGHT_PWM_PIN, 4);
  ledcAttachPin(LEFT_PWM_PIN, 5);

  //跳ばすモーター２つ
  pinMode(THR_RIGHT_DIR_PIN, OUTPUT);
  pinMode(THR_LEFT_DIR_PIN, OUTPUT);
  ledcSetup(6, 4000, 8);
  ledcSetup(7, 4000, 8);
  ledcAttachPin(THR_RIGHT_PWM_PIN, 6);
  ledcAttachPin(THR_LEFT_PWM_PIN, 7);
  
  //ロリコン
  pinMode(X_PIN, INPUT_PULLUP);
  encoder.attachSingleEdge(A_PIN, B_PIN);
  attachInterrupt(digitalPinToInterrupt(X_PIN), handleIndexPulse, RISING); //割り込み

  PS4.begin("08:b6:1F:ed:54:40");
  Serial.println("Ready!");

  myServo.write(90);
}

void loop() {
  int64_t count = encoder.getCount();

  if(!PS4.isConnected()) {
    ledcWrite(4, 0);
    ledcWrite(5, 0); 
    ledcWrite(6, 0);
    ledcWrite(7, 0);
    return;
  }

  int rspeed = PS4.R2Value();
  int lspeed = PS4.L2Value();

  int right_moter = PS4.RStickY();
  int left_moter = PS4.LStickY();
  
  if(PS4.Circle()){
    servo_dir = !servo_dir;
    myServo.write(servo_dir ? 90:10);
    delay(500);
  }

  //車輪のモーター-------------------------------------------------------------
  if(abs(right_moter) > 30){
    ledcWrite(4, abs(right_moter));
    digitalWrite(RIGHT_DIR_PIN, right_moter > 0 ? HIGH:LOW);
  }
  else{
    ledcWrite(4, 0);
  }

  if(abs(left_moter) > 30){
    ledcWrite(5, abs(left_moter));
    digitalWrite(LEFT_DIR_PIN, left_moter > 0 ? LOW:HIGH);
  }
  else{
    ledcWrite(5, 0);
  }

  //跳ばすモーター-------------------------------------------------------------
  if(PS4.Triangle()){
    moter_dir = !moter_dir;
    delay(500);
  }  

  if(abs(rspeed) > 30){
    ledcWrite(6, rspeed);
    digitalWrite(THR_RIGHT_DIR_PIN, moter_dir ? HIGH:LOW);
  }
  else{
    ledcWrite(6, 0);
  }

  if(abs(lspeed) > 30){
    ledcWrite(7, lspeed);
    digitalWrite(THR_LEFT_DIR_PIN, moter_dir ? LOW:HIGH);
  }
  else{
    ledcWrite(7, 0);
  }

  delay(100);
}
