#pragma once
#include <Arduino.h>

#if defined(ARDUINO_AVR_UNO)
#define VRX_PIN A0
#define VRY_PIN A1
#elif defined(ARDUINO_ESP32_DEV)
#define VRX_PIN 34
#define VRY_PIN 35
#else
#error "Unsupported board! Please define VRX_PIN and VRY_PIN for your board."
#endif

extern short dirValue;
extern short throttleValue;

extern byte NEUTRAL_ALLOWANCE;

// 1 for forward, 0 for stop, -1 for backward
extern int8_t leftMotorDirectionForward;
extern short leftMotorSpeed;
// 1 for forward, 0 for stop, -1 for backward
extern int8_t rightMotorDirectionForward;
extern short rightMotorSpeed;
extern short motorMaxSpeed;

extern short joystickMidX;
extern short joystickMaxX;
extern short joystickMidY;
extern short joystickMaxY;