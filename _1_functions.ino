void callback(char* topic, byte* payload, unsigned int length) {
  
  
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
  MQTT_connect();
  String  jsonmessage = GetJsonMessage(message);
  //mqttclient.publish(topic, jsonmessage.c_str()); // disable qos1
  bool sent = box.publish(jsonmessage.c_str());
  Serial.print("Status:");
  Serial.println(sent);
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
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

void reconnect() {
  // Loop until we're reconnected
  while (!pubsubclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pubsubclient.connect(clientName)) {
      Serial.println("connected");
      pubsubclient.publish("outTopic", "hello world");
      
      pubsubclient.subscribe("/office/boxfeed/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(pubsubclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
