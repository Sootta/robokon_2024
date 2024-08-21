#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

//サーボ
Servo myServo;
const int SERVO_PIN = 18;
int servo_deg = 90;

//右のモーター
const int DIR_PIN = 21;
const int PWM_PIN = 26;

void setup() {
  Serial.begin(9600);
  PS4.begin("08:b6:1F:ed:54:40");
  myServo.attach(SERVO_PIN);
  pinMode(DIR_PIN, OUTPUT);
  ledcSetup(3, 10000, 8);
  ledcAttachPin(PWM_PIN, 3);
}

void loop() {
  if(!PS4.isConnected()){
    ledcWrite(3,0);
    return;
  }
  int moter_speed = PS4.RStickY();

  if(PS4.Circle()){
    Serial.println("p");
    servo_deg += 10;
    myServo.write(servo_deg);
    delay(100);
  }
  else if(PS4.Cross()){
    Serial.println("m");
    servo_deg -= 10;
    myServo.write(servo_deg);
    delay(100);
  }
  ledcWrite(3, abs(moter_speed));
  digitalWrite(DIR_PIN, moter_speed > 0 ? HIGH:LOW);

  delay(100);
}

