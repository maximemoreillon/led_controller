void mqtt_config(){
  iot_kernel.mqtt.setCallback(mqtt_message_callback);

  // overwrite default topics
//  iot_kernel.mqtt_status_topic = MQTT_LIGHT_STATUS_TOPIC;
//  iot_kernel.mqtt_command_topic = MQTT_LIGHT_COMMAND_TOPIC;
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

  if(inbound_JSON_message.containsKey("state")){

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
