#include <Arduino.h>
#include "MotorControl.h"

// Pin definitions
const int buttonPin = 2;
MotorControl motorAnkle1(3); // Pin for motor_ankle_1
MotorControl motorHip1(4);  // Pin for motor_hip_1
MotorControl motorHip2(5);  // Pin for motor_hip_2
MotorControl motorAnkle2(6); // Pin for motor_ankle_2

// State variables
bool buttonPressed = false;

void setup() {
    // Configure button pin
    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize motor positions
    motorAnkle1.setPosition(90);
    motorAnkle2.setPosition(90);
    motorHip1.setPosition(90);
    motorHip2.setPosition(90);
}

void loop() {
    // Check if the button is pressed and not already handled
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
        buttonPressed = true;

        // Start moving motor_ankle_1 to 180 degrees
        motorAnkle1.startMotor(180);

        // Wait until all motors reach their final positions
        while (motorAnkle1.isMovingMotor() || 
               motorHip1.isMovingMotor() || 
               motorHip2.isMovingMotor()) {
            if(motorAnkle1.getPosition() == 135) {
                motorHip1.startMotor(180); // Move motor_hip_1 to 180 degrees
                motorHip2.startMotor(0);   // Move motor_hip_2 to 0 degrees
            }
            motorAnkle1.update();
            motorHip1.update();
            motorHip2.update();
            motorAnkle2.update();
        }

        // Move motor_ankle_1 back to 0 degrees
        motorAnkle1.startMotor(0);

        while (motorAnkle1.isMovingMotor()){
            motorAnkle1.update();
        }
    }

    // Reset button state when the button is released
    if (digitalRead(buttonPin) == HIGH) {
        buttonPressed = false;
    }
}