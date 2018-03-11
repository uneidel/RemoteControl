

void setup() 
{
 Serial.begin(115200);  
  WiFiManager wifiManager;
  
  //setupNTP();
  InitButtons();
  LoadSetup();

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_mqtt_server("mqtt", "MQTT Server", mqtt_server, 50);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.autoConnect("SetupHotSpot");
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  //strcpy(bot_token, custom_bot_id.getValue());
  strcpy(mqtt_server, custom_mqtt_server.getValue());
   if (shouldSaveConfig) {
      saveConfig();
   }  
  display.init();
  display.flipScreenVertically();
  display.setLogBuffer(5, 30);
  display.drawString(20,20,Title);
  display.drawLogBuffer(0, 0);
  display.display();
 
  Serial.print("MQTT Server: ");
  Serial.println(mqtt_server);
  SetupPubSub();
  PubSubConnect();
  Serial.println("Setup Done");
}

void InitButtons(){
  pinMode(D2,OUTPUT); // Some Fancy Workaround
  buttonSelect.attachDoubleClick(Maindoubleclick);
  buttonSelect.attachClick(Mainclick);
  buttonSelect.attachLongPressStop(MainlongPress);
  buttonUp.attachClick(UpClick);
  buttonUp.attachLongPressStop(UpClicklongPress);
  buttonDown.attachClick(DownClick);
  pinMode(D1,OUTPUT);// Some Fancy Workaround
  buttonLeft.attachClick(SingleClick);
//  pinMode(D9,OUTPUT);// Some Fancy Workaround
  buttonRight.attachClick(MergeClick);
}

void setupNTP(){
  Serial.println("Connect NTP");
  NTP.begin("0.de.pool.ntp.org", 1, true);
  NTP.setInterval(63);
  
}


void PubSubConnect()
{
  while (!pubsubclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (pubsubclient.connect(clientName,WILL_FEED,1,false, clientName)) {
      
      pubsubclient.subscribe("/office/flurboxin");
      Serial.println("connected");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(pubsubclient.state());
      delay(1000);
    }
  }
}
void SetupPubSub(){
  pubsubclient.setServer(mqtt_server, 1883);
  pubsubclient.setCallback(pubsubCallback);
}


