void web_server_setup() {  

  Serial.println(F("[Web server] Web server initialization"));
  
  web_server.on("/", HTTP_GET, handle_root);

  // Turn those into a POST
  web_server.on("/on", HTTP_GET, handle_turn_on);
  web_server.on("/off", HTTP_GET, handle_turn_off);
  web_server.on("/toggle", HTTP_GET, handle_toggle);
  
  web_server.on("/color", HTTP_GET, handle_color_form);
  web_server.on("/color", HTTP_POST, handle_color_update);
  
  web_server.on("/update", HTTP_GET, handle_update_form);
  web_server.on("/update",HTTP_POST, handle_update, handle_update_upload);
  
  web_server.onNotFound(handle_not_found);
  web_server.begin();
}

void handle_root() {
  String html = pre_main + root_main() + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_color_form() {
  String html = pre_main + color_form() + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_color_update() {

  // Go through query parameters
  if(web_server.hasArg("color")) {
    
    String hexString = web_server.arg("color");
    
    long number = (long) strtol( &hexString[1], NULL, 16);
    
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;
  
    R_channel.duty_when_on = map(r,0,255,0,PWM_MAX_DUTY);
    G_channel.duty_when_on = map(g,0,255,0,PWM_MAX_DUTY);
    B_channel.duty_when_on = map(b,0,255,0,PWM_MAX_DUTY);
  }

  if(web_server.hasArg("w")) {
    W_channel.duty_when_on = web_server.arg("w").toInt();
  }
  
  String color_response = ""
    +color_subheader+
    "<p>OK</p>";
    
  String html = pre_main + color_response + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);

}

void handle_turn_on(){
  turn_on();
  String response = ""
    "<h2>Turning on</h2>"
    "<p>OK</p>";
  
  String html = pre_main + response + post_main;

  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_turn_off(){
  turn_off();
  String response = ""
    "<h2>Turning off</h2>"
    "<p>OK</p>";
    
  String html = pre_main + response + post_main;

  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_toggle(){
  toggle_state();
  
  String response = ""
    "<h2>Toggling lights</h2>"
    "<p>OK</p>";
    
  String html = pre_main + response + post_main;

  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_update_form(){
  String html = pre_main + update_form + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_update(){

  if(Update.hasError()){
    String firmware_update_failure = ""
      +firmware_update_subheader+
      "<p>Upload failed</p>";
      
    String html = pre_main + firmware_update_failure + post_main;
    web_server.sendHeader("Connection", "close");
    web_server.sendHeader("Access-Control-Allow-Origin", "*");
    web_server.send(200, "text/html", html);
  }
  else {
    String firmware_update_success = ""
      +firmware_update_subheader+
      "<p>Upload successful, rebooting...</p>";
        
    String html = pre_main + firmware_update_success + post_main;
    web_server.sendHeader("Connection", "close");
    web_server.sendHeader("Access-Control-Allow-Origin", "*");
    web_server.send(200, "text/html", html);

    // Reboot after 5000ms (5s)
    delay(5000);
    ESP.restart();
  }
  
}

void handle_update_upload() {
  
  HTTPUpload& upload = web_server.upload();
  
  if(upload.status == UPLOAD_FILE_START){
    
    WiFiUDP::stopAll();
    Serial.printf("[Update] uploading new firmware: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    
    //start with max available size
    if(!Update.begin(maxSketchSpace)){
      Serial.println("[Update] Error while starting update");
    }
  }
  else if(upload.status == UPLOAD_FILE_WRITE){
    if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
      Serial.println("[Update] Error while writing new firmware");
    }
  }
  else if(upload.status == UPLOAD_FILE_END){
    if(Update.end(true)){ //true to set the size to the current progress
      Serial.printf("[Update] Update Success: %u \n", upload.totalSize);
    }
    else {
      Serial.println("[Update] Update failed" );
    }
  }
  yield();
}


void handle_not_found() {
  String html = pre_main + not_found + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}
