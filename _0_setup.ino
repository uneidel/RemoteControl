

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
  
  Serial.println("Connect NTP");
  NTP.begin("0.de.pool.ntp.org", 1, true);
  NTP.setInterval(63);
  display.init();
  //PublishMessage("online");
  display.flipScreenVertically();
 display.setLogBuffer(5, 30);

  display.drawString(20,20,Title);
  display.drawLogBuffer(0, 0);
  display.display();
 
  pubsubclient.setServer(mqtt_server, 1883);
  pubsubclient.setCallback(callback);
  pubsubclient.publish(WILL_FEED, "hello world");
  pubsubclient.subscribe(IN_FEED);
  InitButtons();
  Serial.println("Setup Done");
}

void InitButtons(){
  pinMode(D2,OUTPUT); // Some Fancy Workaround
  buttonMain.attachDoubleClick(Maindoubleclick);
  buttonMain.attachClick(Mainclick);
  buttonMain.attachLongPressStop(MainlongPress);
  buttonUp.attachClick(UpClick);
  buttonUp.attachLongPressStop(UpClicklongPress);
  buttonDown.attachClick(DownClick);
  pinMode(D1,OUTPUT);// Some Fancy Workaround
  buttonSingle.attachClick(SingleClick);
  pinMode(D9,OUTPUT);// Some Fancy Workaround
  buttonMerge.attachClick(MergeClick);
}




