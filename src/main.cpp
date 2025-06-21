#include <Arduino.h>

#include "comm.hpp"
#include "controller.hpp"

void setup() {
  Serial.begin(115200);

  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  if (!rf_driver.init()) {
    Serial.println("RF driver initialization failed!");
    while (1);  // Halt the program if RF driver initialization fails
  }

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
  if (!rf_driver.setChannel(CONTROLLER_RADIO_CHANNEL)) {
    Serial.println("Failed to set radio channel");
    while (1);  // Halt if setting the channel fails
  } else {
    Serial.print("Radio channel set to: '");
    Serial.print(CONTROLLER_RADIO_CHANNEL);
    Serial.println("'");
  }

  if (!rf_driver.setRF(RH_NRF24::DataRate250kbps,
                       RH_NRF24::TransmitPower0dBm)) {
    Serial.println("Failed to set data rate and power");
  } else {
    Serial.println("Data rate set to 250kbps and power to 0dBm");
  }
#endif  // CONTROLLER_COMM_MODULE
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
    rightMotorSpeed = map(throttleValue, joystickMidY + NEUTRAL_ALLOWANCE,
                          joystickMaxY, 0, 255);

    leftMotorDirectionForward = 1;
    rightMotorDirectionForward = 1;
  } else if (throttleValue < joystickMidY - NEUTRAL_ALLOWANCE) {
    leftMotorSpeed =
        map(throttleValue, 0, joystickMidY - NEUTRAL_ALLOWANCE, 255, 0);
    rightMotorSpeed =
        map(throttleValue, 0, joystickMidY - NEUTRAL_ALLOWANCE, 255, 0);

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
    rightMotorTurningSpeed =
        map(dirValue, joystickMidX - NEUTRAL_ALLOWANCE, 0, 0, 255);
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
