void led_init(){

  // PWM frequency to prevent whine
  analogWriteFreq(50000);

  R_channel.init();
  G_channel.init();
  B_channel.init();
  W_channel.init();

}

void turn_on(){
  Serial.println(F("[LED] Turning ON"));
  R_channel.turn_on();
  G_channel.turn_on();
  B_channel.turn_on();
  W_channel.turn_on();
  
  iot_kernel.device_state = "on";
  iot_kernel.mqtt_publish_state();
}

void turn_off(){
  Serial.println(F("[LED] Turning OFF"));
  R_channel.turn_off();
  G_channel.turn_off();
  B_channel.turn_off();
  W_channel.turn_off();
  
  iot_kernel.device_state = "off";
  iot_kernel.mqtt_publish_state();
}

void toggle(){
  Serial.println(F("[LED] Toggling state"));
  if(iot_kernel.device_state == "off"){
    turn_on();
  }
  else if(iot_kernel.device_state == "on"){
    turn_off();
  }
}

void apply_color_config(){
  W_channel.duty_when_on = map(colorConfig.w,0,255,0,PWM_MAX_DUTY);
  R_channel.duty_when_on = map(colorConfig.r,0,255,0,PWM_MAX_DUTY);
  G_channel.duty_when_on = map(colorConfig.g,0,255,0,PWM_MAX_DUTY);
  B_channel.duty_when_on = map(colorConfig.b,0,255,0,PWM_MAX_DUTY);
}


void print_color_config(){
  Serial.print("W=");
  Serial.print(colorConfig.w);
  Serial.print("\tR=");
  Serial.print(colorConfig.r);
  Serial.print("\tG=");
  Serial.print(colorConfig.g);
  Serial.print("\tB=");
  Serial.print(colorConfig.b);
  Serial.println("");
}
