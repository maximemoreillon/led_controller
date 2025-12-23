String processor(const String& var) {

  if(var == "DEVICE_NAME") return String(iot_kernel.device_name); 
  else if(var == "WHITE") return String(ledSettings.w);
  else if(var == "RED") return String(ledSettings.r);
  else if(var == "GREEN") return String(ledSettings.g);
  else if(var == "BLUE") return String(ledSettings.b);
  else if(var == "BLUE") return String(ledSettings.b);
  else if(var == "DEFAULT_STATE") {
    if(ledSettings.defaultState) return "checked";
    else return "";
  }
  
  return String();
  
}

void web_server_config() {  

  iot_kernel.http.on("/led-settings", HTTP_GET, handle_led_settings_form);
  iot_kernel.http.on("/led-settings", HTTP_POST, handle_led_settings_update);

  iot_kernel.http.on("/toggle", HTTP_POST, handle_toggle);
  iot_kernel.http.on("/turn_on", HTTP_POST, handle_turn_on);
  iot_kernel.http.on("/turn_off", HTTP_POST, handle_turn_off);
}

void handle_led_settings_form(AsyncWebServerRequest *request) {
  request->send(LittleFS, "/www/led-settings.html", String(), false, processor);
}


void handle_led_settings_update(AsyncWebServerRequest *request) {

  ledSettings.w = request->arg("w").toInt();
  ledSettings.r = request->arg("r").toInt();
  ledSettings.g = request->arg("g").toInt();
  ledSettings.b = request->arg("b").toInt();

  ledSettings.defaultState = request->arg("defaultState") == "on";


  Serial.println("[LED Settings] Read settings from HTTP request");
  apply_color_config();

  ledSettings.writeToSpiffs();


  request->redirect("/led-settings");
  
}

void handle_toggle(AsyncWebServerRequest *request) {
  Serial.println(F("[HTTP] Toggling command received"));
  toggle();
  request->send(200, "text/html", "OK");
}

void handle_turn_on(AsyncWebServerRequest *request) {
  Serial.println(F("[HTTP] Turn on command received"));
  turn_on();
  request->send(200, "text/html", "OK");
}

void handle_turn_off(AsyncWebServerRequest *request) {
  Serial.println(F("[HTTP] Turn off command received"));
  turn_off();
  request->send(200, "text/html", "OK");
}
