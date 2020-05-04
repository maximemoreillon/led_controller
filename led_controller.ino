#include <ESP8266WiFi.h> // Main ESP8266 library
#include <WiFiClient.h>
#include <ArduinoOTA.h> // OTA update library
#include <WiFiUdp.h> // Required for OTA
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#include "credentials.h";
#include "iot_config_home.h";

//#include "bedroom_light.h";
//#include "living_light.h";
#include "kitchen_light.h";

//#include "entrance_light.h";
//#include "test_light.h";

// Pin map
//#define LED_P1_PIN D8 // test
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




// MQTT
#define MQTT_BROKER_ADDRESS IPAddress(192, 168, 1, 2)
#define MQTT_PORT 1883
#define MQTT_QOS 1
#define MQTT_RETAIN true

// Timing
#define ILLUMINANCE_PUBLISH_PERIOD 10000 // [ms] = 10 seconds
#define ILLUMINANCE_MEASUREMENT_PERIOD 100 // [ms] = 0.1 seconds
#define INCREMENT_PERIOD 2 // [ms]

#define FILTER_CONSTANT 0.1

// The PWM values when each channel is on
// Maxes out at 1023
#define PWM_MAX_DUTY 1023
#define R_ON 1000
#define G_ON 0
#define B_ON 0
#define W_ON 950

// Low pass filter for Photoresistor
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


class LedChannel {
  
  private:
    int pin;
    int current_duty;
    int target_duty;
    long last_increment;
    
  public:

    int duty_when_on;
    int duty_when_off;
    boolean on;
    
    LedChannel(int pin, int duty_when_on) {
      this->pin = pin;
      
      this->duty_when_on = duty_when_on;
      this->duty_when_off = 0;

      this->on = false;
      this->target_duty = 0;
      this->current_duty = 0;
      this->last_increment = 0;
    }

    void init(){
      pinMode(this->pin, OUTPUT);
      analogWrite(this->pin, 0);
    }
    
    void loop() {
      
      // Run periodically
      long now = millis();
      if(now - this->last_increment > INCREMENT_PERIOD) {
        this->last_increment = now;

        // Select target duty based on wether On of Off
        if(this->on) this->target_duty = this->duty_when_on;
        else this->target_duty = this->duty_when_off;
        
        int duty_delta = this->target_duty - this->current_duty;
        
        if(this->current_duty < PWM_MAX_DUTY && duty_delta > 0) {
          this->current_duty += 1;
          analogWrite(this->pin, this->current_duty);
        }
        if(this->current_duty > 0 && duty_delta < 0) {
          this->current_duty -= 1;
          analogWrite(this->pin, this->current_duty);
        }
      }
    }

    void turn_on(){
      this->on = true;
    }

    void turn_off(){
      this->on = false;
    }
};

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
  //OTA_setup();
  web_server_setup();

  
  
}

void loop() {
  //ArduinoOTA.handle();
  wifi_connection_manager();
  MQTT_connection_manager();
  web_server.handleClient();
  MQTT_client.loop();

  R_channel.loop();
  G_channel.loop();
  B_channel.loop();
  W_channel.loop();

  //read_PIR();
  read_photoresistor();
  
}
