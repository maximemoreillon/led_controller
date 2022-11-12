#include <Arduino.h>

#define INCREMENT_PERIOD 2 // [ms]
#define PWM_MAX_DUTY 1023

class LedChannel {

  private:
    int pin;
    int target_duty = 0;
    long last_increment = 0;

  public:

    int current_duty = 0;
    int duty_when_on;
    int duty_when_off = 0;
    boolean on = false;

    LedChannel(int pin, int duty_when_on = 1024) {
      this->pin = pin;
      this->duty_when_on = duty_when_on;
    }

    void init(){
      pinMode(this->pin, OUTPUT);
      analogWrite(this->pin, 0);
      
    }

    void loop() {

      // Run periodically every INCREMENT_PERIOD ms
      long now = millis();
      if(now - this->last_increment > INCREMENT_PERIOD) {
        this->last_increment = now;

        // Select target duty based on wether On of Off
        // Off should be a shortcut to all 0
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
