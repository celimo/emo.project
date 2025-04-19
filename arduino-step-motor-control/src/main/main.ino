#include <Arduino.h>
#include "MotorControl.h"

// Pin definitions
const int buttonPin = 2;
// Global variables for each motor start position
const int startPosMotorAnkle1 = 90;
const int startPosMotorAnkle2 = 90;
const int startPosMotorHip1 = 90;
const int startPosMotorHip2 = 90;

MotorControl motorAnkle1(3); // Pin for motor_ankle_1
MotorControl motorHip1(4);  // Pin for motor_hip_1
MotorControl motorHip2(5);  // Pin for motor_hip_2
MotorControl motorAnkle2(6); // Pin for motor_ankle_2

// State variables
bool buttonPressed = false;
enum State { IDLE, MOVE_ANKLE_1, MOVE_HIPS, RETURN_ANKLE_1 };
State currentState = IDLE;

void setup() {
    // Configure button pin
    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize motor positions
    motorAnkle1.setPosition(startPosMotorAnkle1);
    motorAnkle2.setPosition(startPosMotorAnkle2);
    motorHip1.setPosition(startPosMotorHip1);
    motorHip2.setPosition(startPosMotorHip2);
}

void loop() {
    // Continuously update all motors
    motorAnkle1.update();
    motorHip1.update();
    motorHip2.update();
    motorAnkle2.update();

    // Check if the button is pressed
    if (digitalRead(buttonPin) == LOW && !buttonPressed) {
        buttonPressed = true;
        currentState = MOVE_ANKLE_1; // Start the sequence
    }

    // Reset button state when the button is released
    if (digitalRead(buttonPin) == HIGH) {
        buttonPressed = false;
    }

    // State machine to handle motor logic
    switch (currentState) {
        case IDLE:
            // Do nothing, wait for button press
            break;

        case MOVE_ANKLE_1:
            // Start moving motor_ankle_1 to 180 degrees
            if (!motorAnkle1.isMovingMotor()) {
                motorAnkle1.startMotor(180);
            }

            // Check if motor_ankle_1 has reached 135 degrees
            if (motorAnkle1.getPosition() >= 135) {
                motorHip1.startMotor(180); // Move motor_hip_1 to 180 degrees
                motorHip2.startMotor(0);   // Move motor_hip_2 to 0 degrees
                currentState = MOVE_HIPS;
            }
            break;

        case MOVE_HIPS:
            // Wait until all motors reach their final positions
            if (!motorAnkle1.isMovingMotor() &&
                !motorHip1.isMovingMotor() &&
                !motorHip2.isMovingMotor()) {
                motorAnkle1.startMotor(startPosMotorAnkle1); // Move motor_ankle_1 back to its start position
                currentState = RETURN_ANKLE_1;
            }
            break;

        case RETURN_ANKLE_1:
            // Wait until motor_ankle_1 returns to 90 degrees
            if (!motorAnkle1.isMovingMotor()) {
                currentState = IDLE; // Return to idle state
            }
            break;
    }
}