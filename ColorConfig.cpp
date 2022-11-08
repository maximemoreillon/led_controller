#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

class ColorConfig {

//  private:

  public:

    int w;
    int r;
    int g;
    int b;

    ColorConfig() {
 
    }

    void loadFromSpiffs(){
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
      this->w = doc["w"].as<int>();
      this->r = doc["r"].as<int>();
      this->g = doc["g"].as<int>();
      this->b = doc["b"].as<int>();
    
      Serial.println("[Color] Read color config from spiffs");
    }

    void writeToSpiffs(){
      StaticJsonDocument<1024> doc;

      doc["w"] = this->w;
      doc["r"] = this->r;
      doc["g"] = this->g;
      doc["b"] = this->b;
    
    
      File configFile = LittleFS.open("/color.json", "w");
      if (!configFile) {
        Serial.println("[LittleFS] Failed to open config file for writing");
        return;
      }
    
      serializeJson(doc, configFile);
      Serial.println("[SPIFFS] Finished writing color config file");
    }

};
