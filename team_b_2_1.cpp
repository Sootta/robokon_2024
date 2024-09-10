#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

const int MONIROR_SPEED = 9600;

//サーボ
Servo ServoRight, ServoLeft;
const int RIGHT_SERVO_PIN = 18;
const int LEFT_SERVO_PIN = 19;
//右のモーター
const int RIGHT_DIR_PIN = 21;
const int RIGHT_PWM_PIN = 26;
//左のモーター
const int LEFT_DIR_PIN = 22;
const int LEFT_PWM_PIN = 27;
//サーボの初めの角度(degree)
int servo_deg_right = 135;
int servo_deg_left = 45;

void setup() {
    Serial.begin(MONIROR_SPEED);
    PS4.begin("08:b6:1F:ed:54:40");

    ServoRight.attach(RIGHT_SERVO_PIN);
    ServoLeft.attach(LEFT_SERVO_PIN);

    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(LEFT_DIR_PIN, OUTPUT);
    
    ledcSetup(3, 10000, 8);
    ledcSetup(4, 10000, 8);
    ledcAttachPin(RIGHT_PWM_PIN, 3);
    ledcAttachPin(LEFT_PWM_PIN, 4);

    ServoRight.write(servo_deg_right);
    ServoLeft.write(servo_deg_left);
}

void loop() {
    if(!PS4.isConnected() || PS4.PSButton()){
        ledcWrite(3,0);
        ledcWrite(4,0);
        return;
    }

    int right_moter = map(PS4.RStickY(), -128, 128, -100, 100);
    int left_moter = map(PS4.LStickY(), -128, 128, -100, 100);

    if(PS4.Circle() && servo_deg_right < 135){
        servo_deg_right += 10;
        servo_deg_left -= 10;
        ServoRight.write(servo_deg_right);
        ServoLeft.write(servo_deg_left);
        delay(100);
    }
    else if(PS4.Cross() && servo_deg_right > 60){
        servo_deg_right -= 10;
        servo_deg_left += 10;
        ServoRight.write(servo_deg_right);
        ServoLeft.write(servo_deg_left);
        delay(100);
    }
    else if(PS4.Triangle()){
        servo_deg_right = 135;
        servo_deg_left = 45;
        ServoRight.write(servo_deg_right);
        ServoLeft.write(servo_deg_left);
        delay(100);
    }

    if(abs(right_moter)>30){
        ledcWrite(3, abs(right_moter));
        digitalWrite(RIGHT_DIR_PIN, right_moter > 0 ? HIGH:LOW);
    }
    else{
        ledcWrite(3, 0);
    }
    
    if(abs(left_moter)>30){
        ledcWrite(4, abs(left_moter));
        digitalWrite(LEFT_DIR_PIN, left_moter > 0 ? LOW:HIGH);
    }
    else{
        ledcWrite(4, 0);
    }
    delay(100);
}

