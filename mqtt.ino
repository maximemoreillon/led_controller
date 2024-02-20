void mqtt_config(){
  iot_kernel.mqtt.setCallback(mqtt_message_callback);
  rgbCommandTopic = iot_kernel.mqtt_base_topic + "/rgbw/command";
  

  // overwrite default topics
//  iot_kernel.mqtt_status_topic = MQTT_LIGHT_STATUS_TOPIC;
//  iot_kernel.mqtt_command_topic = MQTT_LIGHT_COMMAND_TOPIC;
}

void mqttSubscribe(){
  static boolean last_connection_state = false;
  if(iot_kernel.mqtt.connected() != last_connection_state) {
    last_connection_state = iot_kernel.mqtt.connected();
    if(iot_kernel.mqtt.connected()) {
      Serial.print("[MQTT] Subscribing to RGBW topic ");
      Serial.println(rgbCommandTopic);
      iot_kernel.mqtt.subscribe(rgbCommandTopic.c_str());
      publishRgbw();
    }
  }

}

void mqtt_message_callback(char* topic, byte* payload, unsigned int payload_length) {

  Serial.print("[MQTT] message received on ");
  Serial.print(topic);
  Serial.print(", payload: ");
  for (int i = 0; i < payload_length; i++) Serial.print((char)payload[i]);
  Serial.println("");

  // Create a JSON object to hold the message
  // Note: size is limited by MQTT library
  StaticJsonDocument<MQTT_MAX_PACKET_SIZE> inbound_JSON_message;

  // Copy the message into the JSON object
  deserializeJson(inbound_JSON_message, payload);

  if( strcmp(topic, rgbCommandTopic.c_str()) == 0) {

    if( inbound_JSON_message.containsKey("w") 
      && inbound_JSON_message["r"]
      && inbound_JSON_message["g"]
      && inbound_JSON_message["b"] ) {

      colorConfig.w = inbound_JSON_message["w"];
      colorConfig.r = inbound_JSON_message["r"];
      colorConfig.g = inbound_JSON_message["g"];
      colorConfig.b = inbound_JSON_message["b"];
    
      Serial.println("[Color] Read color config from MQTT payload");
      apply_color_config();
    
      colorConfig.writeToSpiffs();
  
      publishRgbw();
        
    }
    
  }

  else if(inbound_JSON_message.containsKey("state")){

    Serial.println("[MQTTT] Payload is JSON with state");

    // Check what the command is and act accordingly
    char* command = strdup(inbound_JSON_message["state"]);

    if( strcmp(strlwr(command), "on") == 0 ) turn_on();
    else if( strcmp(strlwr(command), "off") == 0 ) turn_off();
    else if( strcmp(strlwr(command), "toggle") == 0 ) toggle();

    free(command);


  }
  else {
    Serial.println("[MQTTT] Payload is NOT JSON with state");
    if(strncmp((char*) payload, "on", payload_length) == 0){
      turn_on();
    }
    else if(strncmp((char*) payload, "off", payload_length) == 0){
      turn_off();
    }
  }

}


void publishRgbw(){
  // Prerpare a JSON message
    StaticJsonDocument<200> outbound_JSON_message;

    outbound_JSON_message["w"] = colorConfig.w;
    outbound_JSON_message["r"] = colorConfig.r;
    outbound_JSON_message["g"] = colorConfig.g;
    outbound_JSON_message["b"] = colorConfig.b;
    


    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.println(F("[MQTT] publish of RGBW state"));
    String topic = iot_kernel.mqtt_base_topic + "/rgbw/status";
    iot_kernel.mqtt.publish(topic.c_str(), JSONmessageBuffer, MQTT_RETAIN);
}
