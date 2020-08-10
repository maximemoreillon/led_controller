void web_server_setup() {  

  Serial.println(F("[Web server] Web server initialization"));
  
  web_server.on("/", HTTP_GET, handle_root);
  web_server.on("/on", HTTP_GET, handle_turn_on);
  web_server.on("/off", HTTP_GET, handle_turn_off);
  web_server.on("/color_form", HTTP_GET, handle_color_form);
  web_server.on("/color", HTTP_GET, handle_color_update);
  web_server.on("/update_form", handle_update_form);
  web_server.on("/update",HTTP_POST, handle_update, handle_update_upload);
  web_server.onNotFound(handle_not_found);
  web_server.begin();
}

void handle_root() {
  String html = pre_main + root_main + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_color_form() {
  String html = pre_main + color_form + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
  

}

void handle_color_update() {

  // Go through query parameters
  for (int i = 0; i < web_server.args(); i++) {
    //if(web_server.argName(i) == "color") R_channel.duty_when_on = web_server.arg(i).toInt();

    if(web_server.argName(i) == "color") {
      String hexString = web_server.arg(i);
      
      long number = (long) strtol( &hexString[1], NULL, 16);
      int r = number >> 16;
      int g = number >> 8 & 0xFF;
      int b = number & 0xFF;

      R_channel.duty_when_on = map(r,0,255,0,PWM_MAX_DUTY);
      G_channel.duty_when_on = map(g,0,255,0,PWM_MAX_DUTY);
      B_channel.duty_when_on = map(b,0,255,0,PWM_MAX_DUTY);
      
    }
    else if(web_server.argName(i) == "w") {
      W_channel.duty_when_on = web_server.arg(i).toInt();

    }
  }

  String color_response = "<h2>Color selection</h2>"
  "<p>OK</p>";
  
  String html = pre_main + color_response + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_turn_on(){
  turn_on();
  String response = "<h2>Turning on</h2>"
  "<p>OK</p>";
  String html = pre_main + response + post_main;

  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}

void handle_turn_off(){
  turn_off();
  String response = "<h2>Turning off</h2>"
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

  String upload_status;
  if(Update.hasError()){
    upload_status = "Upload failed";
  }
  else {
    upload_status = "Upload success";
  }
  String html = pre_main + upload_status + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
  ESP.restart();
  
}

void handle_update_upload() {
  HTTPUpload& upload = web_server.upload();
  if(upload.status == UPLOAD_FILE_START){
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    Serial.printf("Update: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if(!Update.begin(maxSketchSpace)){//start with max available size
      Update.printError(Serial);
    }
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
      Update.printError(Serial);
    }
  } else if(upload.status == UPLOAD_FILE_END){
    if(Update.end(true)){ //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();
}


void handle_not_found() {
  String html = pre_main + not_found + post_main;
  web_server.sendHeader("Connection", "close");
  web_server.sendHeader("Access-Control-Allow-Origin", "*");
  web_server.send(200, "text/html", html);
}
