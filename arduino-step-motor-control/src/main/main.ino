#include <Arduino.h>
#include "MotorControl.h"

/*
LOGIC:
    To walk:
    1. Press button to start the sequence.
    2. Pivot left: Move both ankle to right 180°.
    3. Rotate both hips to the right.
    4. Pivot right: Move both ankle to left 180°.
    5. Rotate both hips to the left.
    6. Cycle from step 2...
    7. Press the button to stop the sequence.
*/

// Pin definitions
const int buttonPin = 2;
// Global variables for each motor start position
const int startPosMotorAnkle1 = 90;
const int startPosMotorAnkle2 = 90;
const int startPosMotorHip1 = 90;
const int startPosMotorHip2 = 90;

MotorControl motorAnkle1(3); // Pin for motor_ankle_1
MotorControl motorHip1(4);   // Pin for motor_hip_1
MotorControl motorHip2(5);   // Pin for motor_hip_2
MotorControl motorAnkle2(6); // Pin for motor_ankle_2

// State variables
bool buttonPressed = false;
enum State
{
    IDLE,
    FEET_PIVOT_LEFT,
    FEET_PIVOT_RIGHT,
    HIPS_ROTATE_LEFT,
    HIPS_ROTATE_RIGHT,
    STOP_SEQUENCE,
    RETURN_FEET,
    RETURN_HIPS
};
State currentState = IDLE;

void setup()
{
    // Configure button pin
    pinMode(buttonPin, INPUT_PULLUP);

    // Initialize motor positions
    motorAnkle1.setPosition(startPosMotorAnkle1);
    motorAnkle2.setPosition(startPosMotorAnkle2);
    motorHip1.setPosition(startPosMotorHip1);
    motorHip2.setPosition(startPosMotorHip2);
}

void loop()
{
    // Continuously update all motors
    updateMotors();

    // Handle button press
    handleButtonPress();

    // Execute state machine logic
    handleStateMachine();
}

// Helper method to update all motors
void updateMotors()
{
    motorAnkle1.update();
    motorHip1.update();
    motorHip2.update();
    motorAnkle2.update();
}

// Helper method to handle button press
void handleButtonPress()
{
    if (digitalRead(buttonPin) == LOW && !buttonPressed)
    {
        buttonPressed = true;
        if (currentState == IDLE)
        {
            currentState = FEET_PIVOT_LEFT; // Start the sequence
        }
        else
        {
            currentState = STOP_SEQUENCE;
        }
    }

    if (digitalRead(buttonPin) == HIGH)
    {
        buttonPressed = false;
    }
}

// Helper method to handle state machine logic
void handleStateMachine()
{
    switch (currentState)
    {
    case IDLE:
        // Do nothing, wait for button press
        break;

    case FEET_PIVOT_LEFT:
        handleFeetPivotLeft();
        break;

    case FEET_PIVOT_RIGHT:
        handleFeetPivotRight();
        break;

    case HIPS_ROTATE_LEFT:
        handleHipsRotateLeft();
        break;

    case HIPS_ROTATE_RIGHT:
        handleHipsRotateRight();
        break;

    case STOP_SEQUENCE:
        handleStopSequence();
        break;

    case RETURN_FEET:
        handleReturnFeetCenter();
        break;

    case RETURN_HIPS:
        handleReturnHipsCenter();
        break;
    }
}

// State-specific logic for FEET_PIVOT_LEFT
void handleFeetPivotLeft()
{
    // If ankle1 is not moving, move both feet to 180 degrees
    if (!motorAnkle1.isMovingMotor())
    {
        motorAnkle1.startMotor(180);
        motorAnkle2.startMotor(180);
    }

    // When ankle1 hits 135 degrees, start moving the hips
    if (motorAnkle1.getPosition() >= 135)
    {
        // Start moving hips before feet pivot stops
        currentState = HIPS_ROTATE_RIGHT;
    }
}

void handleFeetPivotRight()
{
    // If ankle1 is not moving, move both feet to -180 degrees
    if (!motorAnkle2.isMovingMotor())
    {
        motorAnkle1.startMotor(-180);
        motorAnkle2.startMotor(-180);
    }

    // When ankle1 hits 135 degrees, start moving the hips
    if (motorAnkle1.getPosition() <= -135)
    {
        // Start moving hips before feet pivot stops
        currentState = HIPS_ROTATE_LEFT;
    }
}

// State-specific logic for HIPS_ROTATE_RIGHT
void handleHipsRotateRight()
{
    // Start movement
    if (!motorHip1.isMovingMotor() && !motorHip2.isMovingMotor())
    {
        motorHip1.startMotor(180);
        motorHip2.startMotor(180);
    }
    // Check if both hips have reached the target position
    if (motorHip1.getPosition() >= 179 && motorHip2.getPosition() >= 179)
    {
        // Return feet to standing position
        currentState = FEET_PIVOT_RIGHT;
    }
}

void handleHipsRotateLeft()
{
    // Start movement
    if (!motorHip1.isMovingMotor() && !motorHip2.isMovingMotor())
    {
        motorHip1.startMotor(-180);
        motorHip2.startMotor(-180);
    }
    // Check if both hips have reached the target position
    if (motorHip1.getPosition() <= -179 && motorHip2.getPosition() <= -179)
    {
        // Return feet to standing position
        currentState = FEET_PIVOT_LEFT;
    }
}

// State-specific logic for RETURN_FEET
void handleReturnFeetCenter()
{
    // If feet in standing position, change to iddle state
    if (motorAnkle1.getPosition() == 90 && motorAnkle2.getPosition() == 90 &&
        !motorAnkle1.isMovingMotor() && !motorAnkle2.isMovingMotor())
    {
        currentState = IDLE; // Return to idle state
    }
    // If feet are not in standing position, and are not moving, move them back to 90 degrees
    else if (!motorAnkle1.isMovingMotor() && !motorAnkle2.isMovingMotor())
    {
        // Move both feet back to 90 degrees
        motorAnkle1.startMotor(90);
        motorAnkle2.startMotor(90);
    }
}

// State-specific logic for RETURN_HIPS
void handleReturnHipsCenter()
{
    // If hips in standing position, change to iddle state
    if (motorHip1.getPosition() == 90 && motorHip2.getPosition() == 90 &&
        !motorHip1.isMovingMotor() && !motorHip2.isMovingMotor())
    {
        currentState = RETURN_FEET;
    }
    // If hips are not in standing position, and are not moving, move them back to 90 degrees
    else if (!motorHip1.isMovingMotor() && !motorHip2.isMovingMotor())
    {
        // Move both hips back to 90 degrees
        motorHip1.startMotor(90);
        motorHip2.startMotor(90);
    }
}

void handleStopSequence()
{
    currentState = RETURN_HIPS;
}