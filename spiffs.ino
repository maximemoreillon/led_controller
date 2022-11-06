
void get_color_config_from_spiffs(){

  StaticJsonDocument<1024> doc;


  File configFile = LittleFS.open("/color.json", "r");
  if (!configFile) {
    Serial.println("[SPIFFS] Failed to open config file");
    return;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("[SPIFFS] Config file size is too large");
    return;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  configFile.readBytes(buf.get(), size);


  auto error = deserializeJson(doc, buf.get());
  if (error) {
    Serial.println("[SPIFFS] Failed to parse config file");
    return;
  }

  // export file content to custom config structure
  colorConfig.w = doc["w"].as<int>();
  colorConfig.r = doc["r"].as<int>();
  colorConfig.g = doc["g"].as<int>();
  colorConfig.b = doc["b"].as<int>();

  Serial.println("[Color] Read color config from spiffs");
  print_color_config();

}

void write_color_config_to_spiffs(){
  StaticJsonDocument<1024> doc;

  doc["w"] = colorConfig.w;
  doc["r"] = colorConfig.r;
  doc["g"] = colorConfig.g;
  doc["b"] = colorConfig.b;


  File configFile = LittleFS.open("/color.json", "w");
  if (!configFile) {
    Serial.println("[LittleFS] Failed to open config file for writing");
    return;
  }

  serializeJson(doc, configFile);
  Serial.println("[SPIFFS] Finished writing color config file");

}
