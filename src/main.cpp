#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

#define VRX_PIN A0
#define VRY_PIN A1

short dirValue = 0;
short throttleValue = 0;

byte NEUTRAL_ALLOWANCE = 4;

String controlData;

// 1 for forward, 0 for stop, -1 for backward
int8_t leftMotorDirectionForward = 0;
short leftMotorSpeed = 0;
// 1 for forward, 0 for stop, -1 for backward
int8_t rightMotorDirectionForward = 0;
short rightMotorSpeed = 0;
short motorMaxSpeed = 255;

short joystickMidX = 458;
short joystickMaxX = 910;
short joystickMidY = 438;
short joystickMaxY = 910;

// Data pin connects to pin 12
RH_ASK rf_driver;

void setup() {
  Serial.begin(115200);

  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  rf_driver.init();
}

void loop() {
  // "CD: 'LD1LS200RD1RS150'" is the format of the control data
  // LD1LS200 means left motor direction is forward (1) and speed is 200
  controlData = "";

  dirValue = analogRead(VRX_PIN);
  throttleValue = analogRead(VRY_PIN);

  short leftMotorTurningSpeed, rightMotorTurningSpeed;

  // both motors moving forward if throttle Value is the only one that changed
  if (throttleValue > joystickMidY + NEUTRAL_ALLOWANCE) {
    leftMotorSpeed = map(throttleValue, joystickMidY + 2, joystickMaxY, 0, 255);
    rightMotorSpeed =
        map(throttleValue, joystickMidY + NEUTRAL_ALLOWANCE, joystickMaxY, 0, 255);

    leftMotorDirectionForward = 1;
    rightMotorDirectionForward = 1;
  } else if (throttleValue < joystickMidY - NEUTRAL_ALLOWANCE) {
    leftMotorSpeed = map(throttleValue, 0, joystickMidY - NEUTRAL_ALLOWANCE, 255, 0);
    rightMotorSpeed = map(throttleValue, 0, joystickMidY - NEUTRAL_ALLOWANCE, 255, 0);

    leftMotorDirectionForward = -1;
    rightMotorDirectionForward = -1;
  } else {
    leftMotorSpeed = 0;
    rightMotorSpeed = 0;
    leftMotorDirectionForward = 0;
    rightMotorDirectionForward = 0;
  }

  // slower the speed of one
  if (dirValue >= joystickMidX + NEUTRAL_ALLOWANCE) {
    leftMotorTurningSpeed =
        map(dirValue, joystickMidX + 2, joystickMaxX, 0, 255);
    rightMotorTurningSpeed = 0;
  } else if (dirValue < joystickMidX - NEUTRAL_ALLOWANCE) {
    rightMotorTurningSpeed = map(dirValue, joystickMidX - NEUTRAL_ALLOWANCE, 0, 0, 255);
    leftMotorTurningSpeed = 0;
  } else {
    leftMotorTurningSpeed = 0;
    rightMotorTurningSpeed = 0;
  }

  leftMotorSpeed = leftMotorSpeed - rightMotorTurningSpeed;
  rightMotorSpeed = rightMotorSpeed - leftMotorTurningSpeed;

  if (leftMotorSpeed < 0) {
    leftMotorSpeed *= -1;
  }

  if (rightMotorSpeed < 0) {
    rightMotorSpeed *= -1;
  }

  controlData = "LD" + String(leftMotorDirectionForward) + "LS" +
                String(leftMotorSpeed) + "RD" +
                String(rightMotorDirectionForward) + "RS" +
                String(rightMotorSpeed);

  rf_driver.send((uint8_t *)controlData.c_str(), controlData.length());
  rf_driver.waitPacketSent();

  // print all variables to serial monitor
  Serial.print("CD: '");
  Serial.print(controlData);
  Serial.print("'[X]: ");
  Serial.print(dirValue);
  Serial.print(" [Y]: ");
  Serial.print(throttleValue);
  Serial.print(" LeftDir: ");
  Serial.print(leftMotorDirectionForward);
  Serial.print(" LeftSpeed: ");
  Serial.print(leftMotorSpeed);
  Serial.print(" RightDir: ");
  Serial.print(rightMotorDirectionForward);
  Serial.print(" RightSpeed: ");
  Serial.print(rightMotorSpeed);
  Serial.print(" LeftTurningSpeed: ");
  Serial.print(leftMotorTurningSpeed);
  Serial.print(" RightTurningSpeed: ");
  Serial.println(rightMotorTurningSpeed);
}
