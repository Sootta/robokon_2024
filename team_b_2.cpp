#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

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

int servo_deg_right = 90;
int servo_deg_left = 90;

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

}

void loop() {
    if(!PS4.isConnected() || PS4.PSButton()){
        ledcWrite(3,0);
        ledcWrite(4,0);
        return;
    }

    int right_moter = PS4.RStickY();
    int left_moter = PS4.LStickY();

    if(PS4.Circle()){
        Serial.println("p");
        servo_deg_right += 10;
        servo_deg_left -= 10;
        ServoRight.write(servo_deg_right);
        ServoLeft.write(servo_deg_left);
        delay(100);
    }
    else if(PS4.Cross()){
        Serial.println("m");
        servo_deg_right -= 10;
        servo_deg_left += 10;
        ServoRight.write(servo_deg_right);
        ServoLeft.write(servo_deg_left);
        delay(100);
    }

    if(abs(right_moter)>30){
        ledcWrite(3, abs(right_moter / 2));
        digitalWrite(RIGHT_DIR_PIN, right_moter > 0 ? HIGH:LOW);
    }
    else{
        ledcWrite(3, 0);
    }

    if(abs(left_moter)>30){
        ledcWrite(4, abs(left_moter / 2));
        digitalWrite(LEFT_DIR_PIN, left_moter > 0 ? LOW:HIGH);
    }
    else{
        ledcWrite(4, 0);
    }
    delay(100);
}
