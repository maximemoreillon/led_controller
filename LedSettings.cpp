#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

class LedSettings {

//  private:

  public:

    bool defaultState;
    
    int w;
    int r;
    int g;
    int b;

    LedSettings() {
 
    }

    void loadFromSpiffs(){
      StaticJsonDocument<1024> doc;

      File configFile = LittleFS.open("/ledSettings.json", "r");
      if (!configFile) {
        Serial.println("[SPIFFS] Failed to open color config file");
        return;
      }
    
      size_t size = configFile.size();
      if (size > 1024) {
        Serial.println("[SPIFFS] Color config file size is too large");
        return;
      }
    
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);
    
      configFile.readBytes(buf.get(), size);
    
    
      auto error = deserializeJson(doc, buf.get());
      if (error) {
        Serial.println("[SPIFFS] Failed to parse settings file");
        return;
      }
    
      // export file content to custom config structure
      this->w = doc["w"].as<int>();
      this->r = doc["r"].as<int>();
      this->g = doc["g"].as<int>();
      this->b = doc["b"].as<int>();

      this->defaultState = doc["defaultState"];
    
      Serial.println("[Color] Read settings from SPIFFS");
    }

    void writeToSpiffs(){
      StaticJsonDocument<1024> doc;

      doc["w"] = this->w;
      doc["r"] = this->r;
      doc["g"] = this->g;
      doc["b"] = this->b;
      doc["defaultState"] = this->defaultState;
    
    
      File configFile = LittleFS.open("/ledSettings.json", "w");
      if (!configFile) {
        Serial.println("[LittleFS] Failed to settings file for writing");
        return;
      }
    
      serializeJson(doc, configFile);
      serializeJsonPretty(doc, Serial);
      Serial.println("[SPIFFS] Finished writing settings file");
    }

};
