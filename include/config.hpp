#pragma once

#include <Arduino.h>

#define CONTROLLER_IS_JOY_NRF_SHIELD true

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