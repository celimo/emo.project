#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

class MotorControl {
private:
    int pin;          // PWM pin connected to the motor
    int currentPos;   // Current position of the motor (in degrees)
    int targetPos;    // Target position of the motor (in degrees)
    bool isMoving;    // Flag to indicate if the motor is moving

public:
    // Constructor
    MotorControl(int motorPin);

    // Set the motor to a specific position
    void setPosition(int position);

    // Start moving the motor to a target position
    void startMotor(int position);

    // Stop the motor
    void stopMotor();

    // Get the current position of the motor
    int getPosition();

    // Update motor movement (to be called in the loop if needed)
    void update();

    // Check if the motor is currently moving
    bool isMovingMotor(); // New method
};

#endif