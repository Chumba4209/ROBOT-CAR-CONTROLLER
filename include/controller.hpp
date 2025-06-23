#pragma once
#include <Arduino.h>

#include "config.hpp"

#if CONTROLLER_IS_JOY_NRF_SHIELD
#if defined(ARDUINO_AVR_UNO)
#define VRX_PIN A0
#define VRY_PIN A1
#elif defined(ARDUINO_ESP32_DEV)
#define VRX_PIN 34
#define VRY_PIN 35
#else
#error "Unsupported board! Please define VRX_PIN and VRY_PIN for your board."
#endif  // VRX_PIN and VRY_PIN are the analog pins connected to the joystick
#else
#if defined(ARDUINO_AVR_UNO)
#define VRX_PIN A0
#define VRY_PIN A1
#elif defined(ARDUINO_ESP32_DEV)
#define VRX_PIN 34
#define VRY_PIN 35
#else
#error "Unsupported board! Please define VRX_PIN and VRY_PIN for your board."
#endif  // VRX_PIN and VRY_PIN are the analog pins connected to the joystick
#endif  // CONTROLLER_IS_JOY_NRF_SHIELD

extern short dirValue;
extern short throttleValue;

// 1 for forward, 0 for stop, -1 for backward
extern int8_t leftMotorDirectionForward;
extern short leftMotorSpeed;
// 1 for forward, 0 for stop, -1 for backward
extern int8_t rightMotorDirectionForward;
extern short rightMotorSpeed;
extern short motorMaxSpeed;
