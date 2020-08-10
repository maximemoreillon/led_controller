void turn_on(){
  Serial.println(F("[IO] Turning ON"));
  R_channel.turn_on();
  G_channel.turn_on();
  B_channel.turn_on();
  W_channel.turn_on();
  state = "ON";
  MQTT_publish_state();
}

void turn_off(){
  Serial.println(F("[IO] Turning OFF"));
  R_channel.turn_off();
  G_channel.turn_off();
  B_channel.turn_off();
  W_channel.turn_off();
  state = "OFF";
  MQTT_publish_state();
}

void toggle_relay(){
  Serial.println(F("[IO] Toggling state"));
  if(strcmp(state,"OFF") == 0){
    turn_on();
  }
  else if(strcmp(state,"ON") == 0){
    turn_off();
  }
}

void read_PIR(){
  static long last_PIR_reading;
  
  int PIR_reading = digitalRead(PIR_PIN);
  
  if(PIR_reading != last_PIR_reading) {

    // Motion sensor reading changed
    last_PIR_reading = PIR_reading;

    Serial.println(F("[IO] Motion detection state changed"));

    // Prerpare a JSON message
    StaticJsonDocument<200> outbound_JSON_message;

    // Add the motion detection state to the JSON message
    if(PIR_reading == HIGH){
      outbound_JSON_message["state"] = "motion";
      Serial.println(F("[IO] Motion detected"));
    }
    else {
      outbound_JSON_message["state"] = "idle";
      Serial.println(F("[IO] No motion detected"));
    }

    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.println(F("[MQTT] publish of motion detector state"));
    MQTT_client.publish(MQTT_MOTION_STATUS_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}

void read_photoresistor(){

  

  long now = millis();

  static long last_measurement_time;
  if( now - last_measurement_time > ILLUMINANCE_MEASUREMENT_PERIOD) {
    last_measurement_time = now;
    photoresistor_lpf.feed(analogRead(A0));
    
  }

  static long last_publish_time;
  if( now - last_publish_time > ILLUMINANCE_PUBLISH_PERIOD) {
    last_publish_time = now;
    
    // Prerpare a JSON message
    StaticJsonDocument<200> outbound_JSON_message;

    // Add the DHT reading to the JSON message
    outbound_JSON_message["illuminance"] = (String)photoresistor_lpf.output;
    
    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.print(F("[MQTT] publish of illuminance measurement: "));
    Serial.println(photoresistor_lpf.output);
    MQTT_client.publish(MQTT_ILLUMINANCE_TOPIC, JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
