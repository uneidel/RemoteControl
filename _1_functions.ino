void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;

}
void saveConfig(){
 
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
   
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close(); 
}

void LoadSetup(){
if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          
        
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  
}


void pubsubCallback(char* topic, byte* payload, unsigned int length) {
  
   String json = String((char *)payload);
  Serial.print("char");
  Serial.println(json);
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  const char* message    = root["message"];
  const char* sduration   = root["duration"];
  Serial.print("message:");
  Serial.println(message);
  long duration =3000;
  if (sduration != NULL){
    duration =  atof(sduration);
    Serial.print("Duration: "); Serial.println(duration);
  }
  //display message
  ShowMessage(message, duration);
}

void ShowMessage(String message, long duration){
  display.clear();
  if (message.indexOf("$%$") > 0){
          int firstOcc = message.indexOf("$%$");
          String line1 = message.substring(0, firstOcc);
          display.drawString(0,10, line1);
          String line2 = message.substring(firstOcc +3, message.indexOf("$%$", firstOcc +1));
          display.drawString(0,20, line2);
  }
  else if (message.length() > 27){
    display.drawString(0,10,(message.substring(0,26)));
    display.drawString(0,20,(message.substring(27)));
   
  }
  else
  {
    display.drawString(0,10,message);
    
  }
  display.display();
  displaymessage = millis() + duration;
  
}

String GetCurrentTime(bool wait) {
  String datetimestring = "";
  datetimestring = NTP.getTimeDateString();
  if (wait){
    while (datetimestring == "Time not set"){
      datetimestring = NTP.getTimeDateString();
      delay(100);
    }
  }
  return datetimestring;
}




void PrintCurrentTime()
{
  Serial.println(GetCurrentTime(true));
}



void PublishMessage(char* message)
{
  PubSubConnect();
  pubsubclient.publish(OUT_FEED, message);
  
}


char* GetJsonMessage(char* message)
{

  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["DateTime"] = GetCurrentTime(true);
  root["message"] = message;
  char buffer[256];
  root.printTo(buffer, sizeof(buffer));
  return buffer;
}

void CheckCounterAndSleep()
{
 if (millis() > sleeptimeout) // around 5 min.  //perhaps use millis()
  {
    Serial.println("Sleeeeeeeeep");
    PrintCurrentTime();
    display.clear();
    ESP.deepSleep(0); //Forever
    delay(100);  //FIX for deepsleep
  }

}


/*
 String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
*/
