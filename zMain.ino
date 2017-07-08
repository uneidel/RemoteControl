void loop() { 
  
  if (millis() > displaymessage)
  {
     display.clear();
     display.drawString(20,0,Title);
     display.drawString(15,20,GetCurrentTime(false));
     DrawFooter();
     display.display();
  }
   buttonMain.tick();
   buttonUp.tick();
   buttonDown.tick();
   buttonSingle.tick();
   buttonMerge.tick();
   
   if (!pubsubclient.connected()) {
    Serial.println("PubSubClient reconnect"); 
    reconnect();
  }
  pubsubclient.loop();
  
  httpServer.handleClient();

  CheckCounterAndSleep();
}

void DrawFooter(){
  switch(menuState){
    case 0:
      display.drawHorizontalLine(0, 52, 25);
      break;
    case 1:
      display.drawHorizontalLine(42, 52, 30);
      break;
    case 2:
      display.drawHorizontalLine(86, 52, 40);
      break;
    
  }
  display.drawString(44,52, "Music");
  display.drawString(2,52, "Main");
  display.drawString(88,52, "Custom");
  
}


