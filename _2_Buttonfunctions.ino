
  void Mainclick() {
    if (menuState == 2){ menuState = 0; }
    else {   menuState++;  }
    Serial.print("MenuState: ");
    Serial.println(menuState);
    Serial.println("Main Button SinglePress");
} 

void Maindoubleclick() 
{
    
    PublishMessage("Play");
    Serial.println("Main Button DoublePress");
    DrawButtonAction("Play");
} 

void MainlongPress() 
{
    PublishMessage("Stop");
   Serial.println("Main Button LongPressEnd.");
   DrawButtonAction("Stop");
} 



void UpClick()
{
  char* action = "Failure";
 switch(menuState){
  
    case 0:
      action = "Custom";
      break;
    case 1:
      action = "VolumeUp";
      break;
    case 2:
      action = "TempIndoor";
      break;
  }
  // SEND MESSAGE
    Serial.println("UPCLICK");
   PublishMessage(action);
   DrawButtonAction(action);
}

void DownClick()
{
 char* message = "Failure";
 switch(menuState){
    case 0:
      message = "Mute";
      break;
    case 1:
      message = "VolumeDown";
      break;
    case 2:
      message = "TempOutdoor";
      break;
    
  }
  // SEND MESSAGE
   PublishMessage(message);
   DrawButtonAction(message);
}


void MergeClick()
{

  char* message = "Failure";
 switch(menuState){
  
    case 0:
      message = "MergeScan";
      break;
    case 1:
      message = "Previous";
      break;
    case 2:
      message = "ScanImageADF";
      break;
    
  }
  // SEND MESSAGE
   PublishMessage(message);
   DrawButtonAction(message);
}
void SingleClick()
{
  char* message = "Failure";
 switch(menuState){
  
    case 0:
      message = "SingleScan";
      break;
    case 1:
      message = "Next";
      break;
    case 2:
      message = "ScanImageFlatbed";
      break;
   
  }
  // SEND MESSAGE
   PublishMessage(message);
   DrawButtonAction(message);
}



void UpClicklongPress()
{
  //Disable Sleep
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(30,20, "OTA");
  display.setFont(ArialMT_Plain_10);
  String srv= "http://";
  srv += String(WiFi.localIP());
  
  Serial.print("UpdateServer:");
  Serial.println(srv);
  display.drawString(0,47, srv);
  display.display();
  MDNS.begin(clientName);
  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 1234);
  displaymessage = millis() + 10000000;
}


void DrawButtonAction(char* message)
{
   display.clear();
  display.setFont(ArialMT_Plain_24);
  display.drawString(0,20, message);
  display.setFont(ArialMT_Plain_10);
  DrawFooter();
  display.display();
  displaymessage = millis() + 1500;
}

