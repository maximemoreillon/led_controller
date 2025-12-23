/*
 * LED controller
 * 
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */

#include "IotKernel.h"


#include "pin_mapping_v1_0.h" // Kitchen, Living
//#include "pin_mapping_v1_1.h" // Rest

#include "LowPassFilter.cpp" // A custom made class to help with filtering
#include "LedChannel.cpp" // A custom made class to help with LED control
#include "LedSettings.cpp" // A custom made class to help with color configuration

#define DEVICE_TYPE "light"
#define DEVICE_FIRMWARE_VERSION "0.5.1"


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
#define PHOTORESISTOR_FILTER_CONSTANT 0.1

IotKernel iot_kernel(DEVICE_TYPE,DEVICE_FIRMWARE_VERSION); 
LowPassFilter photoresistor_lpf(PHOTORESISTOR_FILTER_CONSTANT);
LedSettings ledSettings;

LedChannel R_channel(R_PIN);
LedChannel G_channel(G_PIN);
LedChannel B_channel(B_PIN);
LedChannel W_channel(W_PIN);

String rgbCommandTopic;


void setup() {

  if(!Serial) Serial.begin(115200);
  Serial.printf("\n\nIoT light v%s initializing...\n",DEVICE_FIRMWARE_VERSION);
  if (!LittleFS.begin()) Serial.println("[SPIFFS] Failed to mount file system");
  else Serial.println("[SPIFFS] File system mounted");
  

  led_init();
  ledSettings.loadFromSpiffs();
  apply_color_config();
  if(ledSettings.defaultState) {
    Serial.println("[LED] defaultState is true so turning ON");
    turn_on_immediatly();
  }
  else Serial.println("[LED] defaultState is false so keeping LED OFF");
  
  iot_kernel.init();
  
  web_server_config();
  mqtt_config();
}



void loop() {
  iot_kernel.loop();
//  mqttSubscribe();
  led_handle();
  read_motion_sensor();
  read_photoresistor();
}
