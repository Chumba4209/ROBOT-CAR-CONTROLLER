#include "comm.hpp"

#if defined(ARDUINO_AVR_UNO)
#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
// Data pin connects to pin 12
RH_ASK rf_driver;
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
RH_NRF24 rf_driver(9, 10);  // CE pin 9, CSN pin 10
#else
#error \
    "Unsupported communication module! Please define CONTROLLER_COMM_MODULE to either CONTROLLER_COMM_MODULE_RF433 or CONTROLLER_COMM_MODULE_NRF24."
#endif  // CONTROLLER_COMM_MODULE
#elif defined(ARDUINO_ESP32_DEV)
#if CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_RF433
RH_ASK rf_driver(2000, 21, 22);
#elif CONTROLLER_COMM_MODULE == CONTROLLER_COMM_MODULE_NRF24
RH_NRF24 rf_driver(21, 22);  // CE pin 21, CSN pin 22
#else
#error \
    "Unsupported communication module! Please define CONTROLLER_COMM_MODULE to either CONTROLLER_COMM_MODULE_RF433 or CONTROLLER_COMM_MODULE_NRF24."
#endif  // CONTROLLER_COMM_MODULE
#endif

String controlData;