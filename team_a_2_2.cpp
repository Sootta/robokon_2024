#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <ESP32Encoder.h>

Servo myServo;
const int SERVO_PIN = 4;
bool servo_dir = true;

//跳ばすモーター２つ
const int THR_RIGHT_DIR_PIN = 23;
const int THR_RIGHT_PWM_PIN = 13;
const int THR_LEFT_DIR_PIN = 19;
const int THR_LEFT_PWM_PIN = 14;
bool moter_dir = true;

//車輪のモーター２つ
const int RIGHT_DIR_PIN = 21;
const int RIGHT_PWM_PIN = 26;

const int LEFT_DIR_PIN = 22;
const int LEFT_PWM_PIN = 27;

//ロリコン
/*
ESP32Encoder encoderRight, encoderLeft;
const int RIGHT_A_PIN = 35;
const int RIGHT_B_PIN = 34;
const int RIGHT_X_PIN = 32;

const int LEFT_A_PIN = 4;
const int LEFT_B_PIN = 33;
const int LEFT_X_PIN = 18;
*/

//int right_count;
//int left_count;
/*
void handleIndexPulseRight() {
  encoderRight.clearCount();
  right_count++;
}
void handleIndexPulseLeft(){
  encoderLeft.clearCount();
  left_count++;
}
*/
void setup() {
  Serial.begin(9600);
 
  //車輪のモーター２つ
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
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
  /*  pinMode(RIGHT_X_PIN, INPUT_PULLUP);
  encoderRight.attachSingleEdge(RIGHT_A_PIN, RIGHT_B_PIN);
  attachInterrupt(digitalPinToInterrupt(RIGHT_X_PIN), handleIndexPulseRight, RISING); //割り込み
  attachInterrupt(digitalPinToInterrupt(LEFT_X_PIN), handleIndexPulseLeft, RISING); 
  right_count = 0;
  left_count = 0;*/

  PS4.begin("48:E7:29:A3:C4:B8");

  myServo.attach(SERVO_PIN);
  myServo.write(90);
}

void loop() {

  if(!PS4.isConnected()) {
    ledcWrite(4, 0);
    ledcWrite(5, 0); 
    ledcWrite(6, 0);
    ledcWrite(7, 0);
    return;
  }
  
  //int64_t count_right = encoderRight.getCount();
  //int64_t count_left = encoderLeft.getCount();

  int right_moter = PS4.RStickY();
  int left_moter = PS4.LStickY();
  
  if(PS4.Circle()){
    servo_dir = !servo_dir;
    myServo.write(servo_dir ? 90:10);
    Serial.println(servo_dir ? 90:10);
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
  int rspeed = map(PS4.R2Value(), 0, 255, 0, 100);
  int lspeed = map(PS4.L2Value(), 0, 255, 0, 100);

  if(abs(rspeed) > 30){
    ledcWrite(6, rspeed);
    digitalWrite(THR_RIGHT_DIR_PIN, LOW);

    ledcWrite(7, rspeed);
    digitalWrite(THR_LEFT_DIR_PIN, HIGH);
  }
  else if(abs(lspeed) > 30){
    ledcWrite(6, lspeed);
    digitalWrite(THR_RIGHT_DIR_PIN, HIGH);

    ledcWrite(7, lspeed);
    digitalWrite(THR_LEFT_DIR_PIN, LOW);
  }
  else{
    ledcWrite(6, 0);
    ledcWrite(7, 0);
  }

  delay(100);
}
