void read_photoresistor(){
  long now = millis();

  static long last_measurement_time;
  if( now - last_measurement_time > ILLUMINANCE_MEASUREMENT_PERIOD) {
    last_measurement_time = now;

    // Do not measure light level if light is on
    // i.e. only measure natural light
    if(R_channel.current_duty > 0) return;
    if(G_channel.current_duty > 0) return;
    if(B_channel.current_duty > 0) return;
    if(W_channel.current_duty > 0) return;
    
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
    String topic = iot_kernel.mqtt_base_topic + "/illuminance";
    iot_kernel.mqtt.publish(topic.c_str(), JSONmessageBuffer, MQTT_RETAIN);
    
  }
}
