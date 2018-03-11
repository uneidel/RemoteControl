#include <Wire.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"
#include "OneButton.h"
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <Bounce2.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>        
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define Title "RemoteControl v0.99"
const uint sleeptimeout = 120000;

WiFiClient client;
#define clientName "remotecontrol"

const char* WILL_FEED="/devices";
const char* OUT_FEED="/office/remoteboxout";
const char* IN_FEED="/office/remoteboxin";
const char* ntpserver ="0.de.pool.ntp.org";
 
bool shouldSaveConfig = false;
char mqtt_server[40];
WiFiClient espClient;
ESP8266WebServer httpServer(1234);
ESP8266HTTPUpdateServer httpUpdater;
PubSubClient pubsubclient(espClient);


SSD1306Spi  display(D8, D4, D6);
// Init Buttons
OneButton buttonSelect(D2, true);
OneButton buttonUp(D0, true);
OneButton buttonDown(D3, true);
OneButton buttonLeft(D1,true);
OneButton buttonRight(3,true);
//Message Stay
long displaymessage= 0;
// Menu State
int menuState=0;




