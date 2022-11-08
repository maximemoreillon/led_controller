String color_to_string(){

  // Currently unused
  
  // int r = R_channel.duty_when_on = map(r,0,255,0,PWM_MAX_DUTY);
  
  uint8_t  red = map(R_channel.duty_when_on,0,PWM_MAX_DUTY,0,255);
  uint8_t  green = map(G_channel.duty_when_on,0,PWM_MAX_DUTY,0,255);
  uint8_t  blue = map(B_channel.duty_when_on,0,PWM_MAX_DUTY,0,255);

  char colorBuffer[24];
  snprintf(colorBuffer, sizeof(colorBuffer), "%02X%02X%02X", red, green, blue);
  
  return "#" + String(colorBuffer);

}
