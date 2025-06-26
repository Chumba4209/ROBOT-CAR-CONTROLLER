#include <Arduino.h>

#include "comm.hpp"
#include "controller.hpp"

void setup() {
  Serial.begin(115200);

  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);

  pinMode(CONTROLLER_HOOTING_PIN, INPUT);

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
  if (!rf_driver.init()) {
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
  if (!rf_driver.begin()) {
#endif  // CONTROLLER_COMM_MODULE
    Serial.println("RF driver initialization failed!");
    while (1);  // Halt the program if RF driver initialization fails
  } else {
    Serial.println("RF driver initialized successfully!");
  }

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
  rf_driver.openWritingPipe(address);
  // The lowest data rate value for more stable communication
  rf_driver.setDataRate(RF24_250KBPS);
  rf_driver.setPALevel(RF24_PA_MIN);
  // Set module as transmitter
  rf_driver.stopListening();
#endif  // CONTROLLER_COMM_MODULE
}

void loop() {
  // "CD: 'LD1LS200RD1RS150H1'" is the format of the control data
  // LD1LS200 means left motor direction is forward (1) and speed is 200
  // RD1RS150 means right motor direction is forward (1) and speed is 150
  // H1 means hooting is active (1)
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

  // Hooting
  hoot = digitalRead(CONTROLLER_HOOTING_PIN);
  if (CONTROLLER_HOOTING_ACTIVE_LOW) {
    hoot = !hoot;
  }

  // It was moving in the opposite direction, so swap the speeds
  if (leftMotorSpeed != rightMotorSpeed) {
    short temp = leftMotorSpeed;
    leftMotorSpeed = rightMotorSpeed;
    rightMotorSpeed = temp;
  }

  controlData = "LD" + String(leftMotorDirectionForward) + "LS" +
                String(leftMotorSpeed) + "RD" +
                String(rightMotorDirectionForward) + "RS" +
                String(rightMotorSpeed) + "H" + String(hoot);

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
  rf_driver.send((uint8_t *)controlData.c_str(), controlData.length());
  rf_driver.waitPacketSent();
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
  if (role) {
    unsigned long start_timer = micros();  // start the timer
    char txtMessage[32];
    controlData.toCharArray(txtMessage, sizeof(txtMessage));
    // transmit & save the report
    bool report = rf_driver.write(txtMessage, strlen(txtMessage) + 1);
    // end the timer
    unsigned long end_timer = micros();

    if (report) {
      Serial.print(F("Transmission successful! "));
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);
      Serial.print(F(" us. Sent: "));
      Serial.println(controlData);
    } else {
      // payload was not delivered
      Serial.println(F("Transmission failed or timed out"));
    }
  }
#endif  // CONTROLLER_COMM_MODULE
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
