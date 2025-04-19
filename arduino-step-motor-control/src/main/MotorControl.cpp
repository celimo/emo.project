#include "MotorControl.h"

MotorControl::MotorControl(int motorPin) {
    pin = motorPin;
    currentPos = 90; // Default to 90 degrees
    targetPos = 90;
    isMoving = false;
    pinMode(pin, OUTPUT);
}

void MotorControl::setPosition(int position) {
    currentPos = position;
    targetPos = position;
    isMoving = false;
    analogWrite(pin, map(position, 0, 180, 0, 255)); // Map degrees to PWM signal
}

void MotorControl::startMotor(int position) {
    targetPos = position;
    isMoving = true;
}

void MotorControl::stopMotor() {
    isMoving = false;
}

int MotorControl::getPosition() {
    return currentPos;
}

void MotorControl::update() {
    if (isMoving) {
        if (currentPos < targetPos) {
            currentPos++;
        } else if (currentPos > targetPos) {
            currentPos--;
        } else {
            isMoving = false; // Reached target position
        }
        analogWrite(pin, map(currentPos, 0, 180, 0, 255)); // Update PWM signal
    }
}

// New method to check if the motor is moving
bool MotorControl::isMovingMotor() {
    return isMoving;
}