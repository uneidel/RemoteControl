#include <Wire.h>
#include <ESP8266WiFi.h>
#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"
#include "OneButton.h"
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <Bounce2.h>
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define Title "Remote Control v0.5"
const char* mqtt_server = "192.168.178.100";
const char* clientName="remoteBox";
const char* ssid     = "fritzbox";
const char* passwd   = "";
const uint sleeptimeout = 120000;

WiFiClient client;
#define WILL_FEED "/devices"
#define OUT_FEED "/office/remoteboxout"
#define IN_FEED "/office/remoteboxin"

 
PubSubClient pubsubclient(client);
Adafruit_MQTT_Client mqtt(&client, mqtt_server, 1883, "", "", "");
Adafruit_MQTT_Publish box = Adafruit_MQTT_Publish(&mqtt, OUT_FEED, MQTT_QOS_1);

ESP8266WebServer httpServer(1234);
ESP8266HTTPUpdateServer httpUpdater;


SSD1306Spi  display(D8, D4, D6);
// Init Buttons
OneButton buttonMain(D2, true);
OneButton buttonUp(D0, true);
OneButton buttonDown(D3, true);
OneButton buttonSingle(D1,true);
OneButton buttonMerge(D9,true);

//Message Stay
long displaymessage= 0;

// Menu State
int menuState=0;




/*
 * Only compiling when set to nodemcu 0.9
 * 
 * 
 */

