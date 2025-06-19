#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

#define VRX_PIN A0

int xValue = 0;

bool motorDirectionForward = true;
short motorSpeed = 0;

short joystickMidX = 457;
short joystickMaxX = 905;

// Data pin connects to pin 12
RH_ASK rf_driver;

void setup() {
  Serial.begin(115200);

  pinMode(VRX_PIN, INPUT);

  rf_driver.init();
}

void loop() {
  // "DIRECTION_FORWARD MOTOR_SPEED"
  // Message to send "1 200"
  String controlData = "";

  xValue = analogRead(VRX_PIN);

  if (xValue >= joystickMidX + 2) {
    motorSpeed = map(xValue, joystickMidX + 2, joystickMaxX, 0, 255);
    motorDirectionForward = true;
  } else if (xValue < joystickMidX - 2) {
    motorSpeed = map(xValue, 0, joystickMidX - 2, 255, 0);
    motorDirectionForward = false;
  } else {
    motorSpeed = 0;
  }

  controlData += (motorDirectionForward ? "1 " : "0 ");
  controlData += String(motorSpeed);

  rf_driver.send((uint8_t *)controlData.c_str(), controlData.length());
  rf_driver.waitPacketSent();
  Serial.print("Sending: ");
  Serial.print(controlData);
  Serial.print(" Raw Data: ");
  Serial.println(xValue);
}
