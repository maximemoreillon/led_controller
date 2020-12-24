/*
 * IoT LED controller
 * 
 * Maxime MOREILLON
 * 
 * Board type: Wemos D1 Mini
 * 
 */
 
// Libraries
#include <ESP8266WiFi.h> // Main ESP8266 library
#include <WiFiClient.h>
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> // Another DNS server to help find the device on a network
#include "uptime_formatter.h"


#include "LedChannel.cpp" // A Custom made class to help with LED control
#include "LowPassFilter.cpp" // A Custom made class to help with filtering


// Environment config
#include "credentials.h"; // Wifi + MQTT credentials
#include "iot_config_home.h"; // MQTT broker URL + connection config

// Device config
//#include "bedroom_light.h";
//#include "living_light.h";
//#include "kitchen_light.h";
//#include "entrance_light.h";
//#include "corridor_light.h";
#include "test_light.h";

#define FIRMWARE_VERSION "0.7.3"

// Pin map
#define LED_P1_PIN D7
#define LED_P2_PIN D8
#define LED_P3_PIN D6
#define LED_P5_PIN D2
#define LED_P4_PIN D5 // Unused due to transistor colliding with holder

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
#define R_ON 1000
#define G_ON 0
#define B_ON 0
#define W_ON 950

// Instanciate objects
ESP8266WebServer web_server(80);
WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);

LowPassFilter photoresistor_lpf(FILTER_CONSTANT);

LedChannel R_channel(R_PIN, R_ON);
LedChannel G_channel(G_PIN, G_ON);
LedChannel B_channel(B_PIN, B_ON);
LedChannel W_channel(W_PIN, W_ON);

char* state = "OFF";

void setup() {

  
  
  //LED_init();
  R_channel.init();
  G_channel.init();
  B_channel.init();
  W_channel.init();
  
  pinMode(PIR_PIN, INPUT);

  // Serial init
  Serial.begin(115200);
  Serial.println();
  Serial.println(); // Separate serial stream from initial gibberish
  
  wifi_setup();
  MQTT_setup();
  OTA_setup();
  web_server_setup();
  MDNS.begin(HOSTNAME);
  
}

void loop() {
  MDNS.update();
  ArduinoOTA.handle();
  wifi_connection_manager();
  MQTT_connection_manager();
  web_server.handleClient();
  MQTT_client.loop();

  R_channel.loop();
  G_channel.loop();
  B_channel.loop();
  W_channel.loop();

  read_PIR();
  read_photoresistor();
  
}
