#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Encoder.h>
#include <ESP32Servo.h>

const int SERIAL_SPEED = 9600;

const int PWM_RESOLUTION = 8;

const int RIGHT_DIR_PIN = 21;
const int RIGHT_PWM_PIN = 26;
const int LEFT_DIR_PIN = 22;
const int LEFT_PWM_PIN = 27;

const int THR_DIR_PIN = 23;
const int THE_PWM_PIN = 13;

const int SERVO_PIN = 32;
Servo myServo;
int deg = 90;

void setup() {
    Serial.begin(SERIAL_SPEED);
    PS4.begin("48:E7:29:A3:C2:24");

    pinMode(RIGHT_DIR_PIN, OUTPUT);
    pinMode(LEFT_DIR_PIN, OUTPUT);
    pinMode(THR_DIR_PIN, OUTPUT);

    ledcSetup(1, 10000, PWM_RESOLUTION);
    ledcSetup(2, 10000, PWM_RESOLUTION);
    ledcSetup(3, 10000, PWM_RESOLUTION);
    ledcAttachPin(RIGHT_PWM_PIN, 1);
    ledcAttachPin(LEFT_PWM_PIN, 2);
    ledcAttachPin(THE_PWM_PIN, 3);

    myServo.attach(SERVO_PIN);
    myServo.write(deg);
}

void loop() {
    if(!PS4.isConnected()){
        ledcWrite(1,0);
        ledcWrite(2,0);
        ledcWrite(3,0);
        return;
    }

    if(PS4.Circle()){
        deg += 10;
        myServo.write(deg);
        delay(100);
    }
    else if(PS4.Cross()){
        deg -= 10;
        myServo.write(deg);
        delay(100);       
    }

    if(PS4.R2Value() > 30){
        digitalWrite(THR_DIR_PIN, HIGH);
        ledcWrite(3, PS4.R2Value() / 2);
    }
    else if(PS4.L2Value() > 30){
        digitalWrite(THR_DIR_PIN, LOW);
        ledcWrite(3, PS4.L2Value() / 2);
    }
    else{
        ledcWrite(3, 0);
    }

    int rspeed = PS4.RStickY();
    int lspeed = PS4.LStickY();

    if(abs(rspeed) > 30){
        ledcWrite(1, abs(rspeed));
        digitalWrite(RIGHT_DIR_PIN, rspeed > 0 ? HIGH:LOW);
    }
    else{
        ledcWrite(1, 0);
    }
    if(abs(lspeed) > 30){
        ledcWrite(2, abs(lspeed));
        digitalWrite(LEFT_DIR_PIN, rspeed > 0 ? LOW:HIGH);
    }
    else{
        ledcWrite(2, 0);
    }

    delay(100);
}

