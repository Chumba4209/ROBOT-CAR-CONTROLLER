#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "config.hpp"

#define CONTROLLER_COMM_MODULE_RF433 433
#define CONTROLLER_COMM_MODULE_NRF24 24

// Change this to the desired communication module
#define CONTROLLER_COMM_MODULE CONTROLLER_COMM_MODULE_NRF24

#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
#include <RH_ASK.h>
#define MAX_RECV_MESSAGE_LENGTH RH_ASK_MAX_MESSAGE_LEN
#define RF433_DATA_PIN 12  // Not used, just for reference

extern RH_ASK rf_driver;
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
#include <RF24.h>
#include <RHSoftwareSPI.h>
#include <nRF24L01.h>

#define NRF24L01_CE_PIN 9
#define NRF24L01_CSN_PIN 10

#define MAX_RECV_MESSAGE_LENGTH RH_NRF24_MAX_MESSAGE_LEN
#define CONTROLLER_RADIO_CHANNEL 1

// extern RH_NRF24 rf_driver;
extern RF24 rf_driver;
extern const uint8_t address[];
#else
#error \
    "Unsupported communication module! Please define CONTROLLER_COMM_MODULE to either CONTROLLER_COMM_MODULE_RF433 or CONTROLLER_COMM_MODULE_NRF24."
#endif  // CONTROLLER_COMM_MODULE

extern String controlData;
