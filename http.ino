String processor(const String& var) {

  if(var == "DEVICE_NAME") return String(iot_kernel.device_name); 
  else if(var == "WHITE") return String(colorConfig.w);
  else if(var == "RED") return String(colorConfig.r);
  else if(var == "GREEN") return String(colorConfig.g);
  else if(var == "BLUE") return String(colorConfig.b);
  
  return String();
  
}

void web_server_config() {  
  iot_kernel.http.on("/color", HTTP_GET, handle_color_form);
  iot_kernel.http.on("/color", HTTP_POST, handle_color_update);
  
  iot_kernel.http.on("/toggle", HTTP_POST, handle_toggle);
//  iot_kernel.http.on("/turn_on", HTTP_POST, handle_toggle);
//  iot_kernel.http.on("/turn_off", HTTP_POST, handle_toggle);
}

void handle_color_form(AsyncWebServerRequest *request) {
  request->send(LittleFS, "/www/color.html", String(), false, processor);
}


void handle_color_update(AsyncWebServerRequest *request) {

  colorConfig.w = request->arg("w").toInt();
  colorConfig.r = request->arg("r").toInt();
  colorConfig.g = request->arg("g").toInt();
  colorConfig.b = request->arg("b").toInt();

  Serial.println("[Color] Read color config from HTTP request");
  print_color_config();

  apply_color_config();

  write_color_config_to_spiffs();
//
//
//  String hexString = request->arg("color");
//  
//  long number = (long) strtol( &hexString[1], NULL, 16);
//  
//  int r = number >> 16;
//  int g = number >> 8 & 0xFF;
//  int b = number & 0xFF;
//
//  R_channel.duty_when_on = map(r,0,255,0,PWM_MAX_DUTY);
//  G_channel.duty_when_on = map(g,0,255,0,PWM_MAX_DUTY);
//  B_channel.duty_when_on = map(b,0,255,0,PWM_MAX_DUTY);


  request->redirect("/color");
  
}

void handle_toggle(AsyncWebServerRequest *request) {
  Serial.println(F("[HTTP] Toggling command received"));
  toggle();
  request->send(200, "text/html", "OK");
  
}
