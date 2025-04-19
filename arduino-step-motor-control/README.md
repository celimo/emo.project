# Arduino Step Motor Control

This project controls four step motors: one for the ankle and two for the hips. The motors are controlled based on button presses, allowing for coordinated movements.

## Project Overview

- **Motor_ankle_1** starts at 0 degrees and can move to 90 degrees and then to 180 degrees.
- **Motor_hip_1** and **Motor_hip_2** start at 180 degrees and move in opposing directions when activated.
- The system is designed to return **motor_ankle_1** to 0 degrees after completing its movements.

## Files Structure

- `src/main.ino`: The main Arduino sketch that initializes the motors and button, and contains the loop for checking button presses and controlling motor movements.
- `lib/MotorControl/MotorControl.h`: Header file defining the `MotorControl` class with methods for controlling motor movements.
- `lib/MotorControl/MotorControl.cpp`: Implementation of the `MotorControl` class methods, handling motor movements and timing.

## Setup Instructions

1. **Hardware Requirements**:
   - Arduino board (e.g., Arduino Uno)
   - Four step motors
   - Motor driver (compatible with the step motors)
   - Push button
   - Resistors (for button pull-down)
   - Breadboard and jumper wires

2. **Wiring Diagram**:
   - Connect the step motors to the motor driver according to the driver's specifications.
   - Connect the push button to a digital pin on the Arduino with a pull-down resistor.

3. **Software Requirements**:
   - Arduino IDE installed on your computer.
   - Ensure you have the necessary libraries for controlling step motors.

## How to Use

1. Upload the `main.ino` sketch to your Arduino board.
2. Press the button to start the sequence:
   - **motor_ankle_1** will move to 90 degrees.
   - **motor_hip_1** and **motor_hip_2** will activate and move in opposite directions until **motor_ankle_1** reaches 180 degrees.
   - After all motors stop, **motor_ankle_1** will return to 0 degrees.

## Additional Notes

- Ensure that the power supply for the motors is sufficient to handle their requirements.
- Adjust the motor speed and step settings in the code as necessary for your specific motors and application.