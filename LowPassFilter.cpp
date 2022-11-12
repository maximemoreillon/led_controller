#include <Arduino.h>

class LowPassFilter {
  
//  private:
    
  public:
    float output;
    float filter_constant; // if set to 1, no filtering
    
    LowPassFilter(float filter_constant) {
      this->output = 0;
      this->filter_constant = filter_constant;
    }
    
    void feed(float input) {
      this->output = (this->output)*(1.00 - this->filter_constant) + input * (this->filter_constant);
    }
};
