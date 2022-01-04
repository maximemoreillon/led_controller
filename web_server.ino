String processor(const String& var) {
  if(var == "COLOR") return color_to_string();
  else if(var == "W_DUTY") return String(W_channel.duty_when_on);
  else if(var == "PWM_MAX_DUTY") return String(PWM_MAX_DUTY);
  return String();
}

void web_server_setup() {  
  iot_kernel.http.on("/color", HTTP_GET, handle_color_form);
  iot_kernel.http.on("/color", HTTP_POST, handle_color_update);
}

void handle_color_form(AsyncWebServerRequest *request) {
  request->send(LittleFS, "/www/color.html", String(), false, processor);
}

void handle_color_update(AsyncWebServerRequest *request) {
    
  String hexString = request->arg("color");
  
  long number = (long) strtol( &hexString[1], NULL, 16);
  
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  R_channel.duty_when_on = map(r,0,255,0,PWM_MAX_DUTY);
  G_channel.duty_when_on = map(g,0,255,0,PWM_MAX_DUTY);
  B_channel.duty_when_on = map(b,0,255,0,PWM_MAX_DUTY);

  W_channel.duty_when_on = request->arg("w").toInt();

  request->redirect("/color");
  


}
