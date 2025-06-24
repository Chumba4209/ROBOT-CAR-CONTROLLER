#pragma once

#include <Arduino.h>

#define CONTROLLER_IS_JOY_NRF_SHIELD true

#if CONTROLLER_IS_JOY_NRF_SHIELD
#define CONTROLLER_HOOTING_PIN 3
#define CONTROLLER_HOOTING_ACTIVE_LOW true
#else
#if defined(ARDUINO_AVR_UNO)
#define CONTROLLER_HOOTING_PIN 3
#define CONTROLLER_HOOTING_ACTIVE_LOW false
#elif defined(ARDUINO_ESP32_DEV)
#define CONTROLLER_HOOTING_PIN 2
#define CONTROLLER_HOOTING_ACTIVE_LOW true
#else
#error \
    "Unsupported board! Please define CONTROLLER_HOOTING_PIN and CONTROLLER_HOOTING_ACTIVE_LOW for your board."
#endif  // CONTROLLER_HOOTING_PIN and CONTROLLER_HOOTING_ACTIVE_LOW
#endif  // CONTROLLER_IS_JOY_NRF_SHIELD

// JOYSTICK CONFIG
#define NEUTRAL_ALLOWANCE 4

#define joystickMidX 509
#define joystickMaxX 1023
#define joystickMidY 526
#define joystickMaxY 1023

// NRF24L01 PIN CONFIG
#define NRF24L01_CE_PIN 9
#define NRF24L01_CSN_PIN 10

// Used to control whether this node is sending or receiving
#define role true  // true = TX role, false = RX role