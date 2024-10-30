#include <Servo.h>

// Define servo objects for each joint
Servo baseServo;
Servo armServo;
Servo elbowServo;
Servo wristServo;
Servo tiltServo;
Servo gripperServo;

// Pin assignments for each servo
const int basePin = 2;
const int armPin = 3;
const int elbowPin = 4;
const int wristPin = 5;
const int tiltPin = 6;
const int gripperPin = 7;

// Input pins for joysticks, potentiometers
const int joystick1XPin = A0; // Joystick 1 X-axis (Base)
const int joystick1YPin = A1; // Joystick 1 Y-axis (Arm)
const int joystick2XPin = A2; // Joystick 2 X-axis (Elbow)
const int joystick2YPin = A3; // Joystick 2 Y-axis (Wrist rotation)
const int tiltPotPin = A4;    // Potentiometer for wrist tilt
const int gripperPotPin = A5; // Potentiometer for gripper control

// Initial servo angles
int baseAngle = 90;
int armAngle = 90;
int elbowAngle = 90;
int wristAngle = 90;
int tiltAngle = 90;
int gripperAngle = 90; 

// Speed control variables
const int incrementDelay = 10;  // Milliseconds between each increment
const int stepSize = 2;         // How much to move the servo in each step

// Deadzone to avoid jittering
const int deadZone = 30; 

void setup() {
  // Attach servos to their respective pins
  baseServo.attach(basePin);
  armServo.attach(armPin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
  tiltServo.attach(tiltPin);
  gripperServo.attach(gripperPin);
  
  // Set initial servo positions
  baseServo.write(baseAngle);
  armServo.write(armAngle);
  elbowServo.write(elbowAngle);
  wristServo.write(wristAngle);
  tiltServo.write(tiltAngle);
  gripperServo.write(gripperAngle);

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read joystick values for base and arm control
  int joystick1X = analogRead(joystick1XPin);
  int joystick1Y = analogRead(joystick1YPin);

  // Read joystick values for elbow and wrist control
  int joystick2X = analogRead(joystick2XPin);
  int joystick2Y = analogRead(joystick2YPin);

  // Update base servo angle with a dead zone
  updateServo(baseServo, baseAngle, map(joystick1X, 0, 1023, 0, 180), stepSize);

  // Update arm servo angle with a dead zone
  updateServo(armServo, armAngle, map(joystick1Y, 0, 1023, 0, 180), stepSize);

  // Update elbow servo angle with a dead zone
  updateServo(elbowServo, elbowAngle, map(joystick2X, 0, 1023, 0, 180), stepSize);

  // Update wrist rotation angle with a dead zone
  updateServo(wristServo, wristAngle, map(joystick2Y, 0, 1023, 0, 180), stepSize);

  // Read potentiometer value and update wrist tilt servo
  tiltAngle = map(analogRead(tiltPotPin), 0, 1023, 0, 180);
  tiltServo.write(tiltAngle);

  // Read potentiometer value for gripper control and update gripper servo
  gripperAngle = map(analogRead(gripperPotPin), 0, 1023, 0, 180);
  gripperServo.write(gripperAngle);

  // Add a small delay to control the update speed
  delay(incrementDelay);
}

// Function to update servo position smoothly with a dead zone
void updateServo(Servo &servo, int &currentAngle, int targetAngle, int stepSize) {
  // Check if the difference between the current and target angle is greater than the dead zone
  if (abs(targetAngle - currentAngle) > deadZone) {
    // Move towards the target angle in steps
    if (targetAngle > currentAngle) {
      currentAngle = min(currentAngle + stepSize, targetAngle);
    } else if (targetAngle < currentAngle) {
      currentAngle = max(currentAngle - stepSize, targetAngle);
    }
    servo.write(currentAngle); // Update the servo position
  }
}
