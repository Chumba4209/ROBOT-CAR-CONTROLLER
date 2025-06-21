#include "controller.hpp"

short dirValue = 0;
short throttleValue = 0;

byte NEUTRAL_ALLOWANCE = 4;

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