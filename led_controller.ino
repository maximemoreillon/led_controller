#include <ESP8266WiFi.h> // Main ESP8266 library
#include <WiFiClient.h>
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#include "credentials.h";
#include "iot_config_home.h";
#include "bedroom_light.h";

// Pin map
#define LED_P1_PIN D7 // Real
//#define LED_P2_PIN D8
//#define LED_P1_PIN D8 // test

#define PHOTORESISTOR_PIN A0
#define PIR_PIN D1


// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_LAST_WILL "{\"state\":\"disconnected\"}"
#define MQTT_QOS 1
#define MQTT_RETAIN true

#define ILLUMINANCE_PUBLISH_PERIOD 60000 // [ms] = 60 seconds
#define ILLUMINANCE_MEASUREMENT_PERIOD 100 // [ms] = 0.1 seconds

#define FILTER_CONSTANT 0.1

class LowPassFilter {
  
  private:
    float filter_constant;
    
  public:
    float output;
    
    LowPassFilter(float filter_constant) {
      this->output = 0;
      this->filter_constant = filter_constant;
    }
    
    void feed(float input) {
      this->output = (this->output)*(1.00 - this->filter_constant) + input * (this->filter_constant);
    }
};

// Instanciate objects
ESP8266WebServer web_server(80);
WiFiClient wifi_client;
PubSubClient MQTT_client(wifi_client);

LowPassFilter photoresistor_lpf(FILTER_CONSTANT);

int led_target = 0;
char* state = "ON";


void setup() {
  
  digitalWrite(LED_P1_PIN,LOW);
  pinMode(LED_P1_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  // Mandatory initial delay
  delay(10);

  // Serial init
  Serial.begin(115200);
  Serial.println();
  Serial.println(); // Separate serial stream from initial gibberish
  
  

  wifi_setup();
  MQTT_setup();
  //OTA_setup();
  web_server_setup();

  
  
}

void loop() {
  //ArduinoOTA.handle();
  wifi_connection_manager();
  MQTT_connection_manager();
  web_server.handleClient();
  MQTT_client.loop();

  handle_LED();
  //read_PIR();
  read_photoresistor();
  
}
