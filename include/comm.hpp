#pragma once

#include <Arduino.h>
#include <SPI.h>

#define CONTROLLER_COMM_MODULE_RF433 433
#define CONTROLLER_COMM_MODULE_NRF24 24

#ifndef CONTROLLER_COMM_MODULE
// Change this to select the communication module
#define CONTROLLER_COMM_MODULE CONTROLLER_COMM_MODULE_RF433
#endif

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
#include <RH_ASK.h>
extern RH_ASK rf_driver;
#define MAX_RECV_MESSAGE_LENGTH RH_ASK_MAX_MESSAGE_LEN
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
#include <RH_NRF24.h>
extern RH_NRF24 rf_driver;
#define MAX_RECV_MESSAGE_LENGTH RH_NRF24_MAX_MESSAGE_LEN
#define CONTROLLER_RADIO_CHANNEL 1
#else
#error \
    "Unsupported communication module! Please define CONTROLLER_COMM_MODULE to either CONTROLLER_COMM_MODULE_RF433 or CONTROLLER_COMM_MODULE_NRF24."
#endif

extern String controlData;