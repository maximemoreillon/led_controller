void motion_sensor_init(){
    pinMode(PIR_PIN, INPUT);

}

void read_motion_sensor(){
  static long last_PIR_reading;
  
  int PIR_reading = digitalRead(PIR_PIN);
  
  if(PIR_reading != last_PIR_reading) {

    // Motion sensor reading changed
    last_PIR_reading = PIR_reading;

    Serial.println(F("[Motion] Motion detection state changed"));

    // Prerpare a JSON message
    StaticJsonDocument<200> outbound_JSON_message;

    // Add the motion detection state to the JSON message
    if(PIR_reading == HIGH){
      outbound_JSON_message["state"] = "motion";
      Serial.println(F("[Motion] Motion detected"));
    }
    else {
      outbound_JSON_message["state"] = "idle";
      Serial.println(F("[Motion] No motion detected"));
    }

    // Serialize JSON into a char array
    char JSONmessageBuffer[100];
    serializeJson(outbound_JSON_message, JSONmessageBuffer, sizeof(JSONmessageBuffer));

    // Send the char array
    Serial.println(F("[MQTT] publish of motion detector state"));
    String topic = iot_kernel.mqtt_base_topic + "/motion";
    iot_kernel.mqtt.publish(topic.c_str(), JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
