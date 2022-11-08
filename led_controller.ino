/*
 * LED controller
 * 
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

#include "IotKernel.h"

//#include "pin_mapping_v1_0.h"
#include "pin_mapping_v1_1.h"

#include "LedChannel.cpp" // A custom made class to help with LED control
#include "LowPassFilter.cpp" // A custom made class to help with filtering
#include "ColorConfig.cpp" // A custom made class to help with color configuration

#define DEVICE_TYPE "light"
#define DEVICE_FIRMWARE_VERSION "0.2.4"


#define PHOTORESISTOR_PIN A0
#define PIR_PIN D1

// Assignment of color channels to pins
#define W_PIN LED_P1_PIN
#define R_PIN LED_P2_PIN
#define G_PIN LED_P3_PIN
#define B_PIN LED_P4_PIN


// Timing
#define ILLUMINANCE_PUBLISH_PERIOD 60000 // [ms] = 60 seconds
#define ILLUMINANCE_MEASUREMENT_PERIOD 100 // [ms] = 0.1 seconds
#define FILTER_CONSTANT 0.1

// The PWM values when each channel is on
#define R_ON 0
#define G_ON 0
#define B_ON 0
#define W_ON 950



IotKernel iot_kernel(DEVICE_TYPE,DEVICE_FIRMWARE_VERSION); 
LowPassFilter photoresistor_lpf(FILTER_CONSTANT);
ColorConfig colorConfig;

LedChannel R_channel(R_PIN, R_ON);
LedChannel G_channel(G_PIN, G_ON);
LedChannel B_channel(B_PIN, B_ON);
LedChannel W_channel(W_PIN, W_ON);


void setup() {

  led_init();
  iot_kernel.init();

  mqtt_config();
  web_server_config();

  colorConfig.loadFromSpiffs();
  apply_color_config();

  

}



void loop() {
  iot_kernel.loop();
  led_handle();
  read_motion_sensor();
  read_photoresistor();
}
