#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

//サーボ
Servo ServoRight, ServoLeft;
const int RIGHT_SERVO_PIN = 25;
const int LEFT_SERVO_PIN = 26;
//右のモーター
const int RIGHT_DIR_PIN = 21;
const int RIGHT_PWM_PIN = 0;
//左のモーター
const int LEFT_DIR_PIN = 21;
const int LEFT_PWM_PIN = 2;

int servo_deg = 90;

void setup() {
    Serial.begin(9600);
    PS4.begin("08:b6:1F:ed:54:40");

    ServoRight.attach(RIGHT_SERVO_PIN);
    ServoLeft.attach(LEFT_SERVO_PIN);

    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(LEFT_DIR_PIN, OUTPUT);
    
    ledcSetup(3, 10000, 8);
    ledcSetup(4, 10000, 8);
    ledcAttachPin(RIGHT_PWM_PIN, 3);
    ledcAttachPin(LEFT_PWM_PIN, 4);

    ServoRight.write(90);
    ServoLeft.write(90);
}

void loop() {
    if(!PS4.isConnected()){
        ledcWrite(3,0);
        ledcWrite(4,0);
        return;
    }
    int right_moter = PS4.RStickY();
    int left_moter = PS4.LStickY();

    if(PS4.Circle()){
        servo_deg++;
        ServoRight.write(servo_deg);
        ServoLeft.write(servo_deg);
        delay(100);
    }
    else if(PS4.Cross()){
        servo_deg--;
        ServoRight.write(servo_deg);
        ServoLeft.write(servo_deg);
        delay(100);
    }

    ledcWrite(3, abs(right_moter));
    ledcWrite(4, abs(left_moter));

    digitalWrite(RIGHT_DIR_PIN, right_moter > 0 ? HIGH:LOW);
    digitalWrite(LEFT_DIR_PIN, left_moter > 0 ? LOW:HIGH);

    delay(100);
}

