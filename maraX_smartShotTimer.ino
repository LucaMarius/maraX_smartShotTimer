// ----- Pinning ----------------------------------------------------------
//Display:
//D1 - SCK
//D2 - SDA
//
//Serial Port:
//D5 - RX (Pin4 @ MaraX)
//D6 - TX (Pin3 @ MaraX)
//
//Pump:
//D7 - Switching GND
//
//Push button:
//D3 - Switching GND


// ----- Includes ----------------------------------------------------------
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
//#include <ESP8266TimerInterrupt.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include "maraX_credentials.h"


// ----- User Defines and Credentials --------------------------------------
#define MQTT_DEFAULT_CYCLE 5000 //ms
#define MQTT_WOKRING_CYCLE 1000 //ms
#define SLEEPTIME 10000 // ms, shutdown display time = serial timeout + sleeptime 
                        // flag raised after no pump changes and machine off (also see serial timeout)

#define DEBUG 1
#define DEBUG_WIFI 0
#define DEBUG_PARSER 0

// ----- check defines ----------------------------------------------------
#if defined(ssid) && defined (wpa2) 
#define ONLINEMODE true
#else
#define ONLINEMODE false
#define ssid "ssid"
#define wpa2 "wpa2"
#endif

#ifdef MQTT_SERVER
#define MQTTMODE true
#else 
#define MQTTMODE false 
#define MQTT_SERVER ""
#endif

#ifndef MQTT_PORT 
#define MQTT_PORT 1883
#endif 

#ifndef MQTT_USER
#define MQTT_USER ""
#endif
#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD ""
#endif


// ----- Timer -------------------------------------------------------------
uint8_t timerCount = 0;
int prevTimerCount = 0;
bool timerStarted = false;
long timerStartMillis = 0;
long timerStopMillis = 0;
long timerDisplayOffMillis = 0;
long serialUpdateMillis = 0;

// Select a Timer Clock
#define USING_TIM_DIV1 false           // for shortest and most accurate timer
#define USING_TIM_DIV16 false           // for medium time and medium accurate timer
#define USING_TIM_DIV256 true            // for longest timer but least accurate. Default

// Init ESP8266 only and only Timer 1
//ESP8266Timer ITimer;

#define TIMER_INTERVAL_MS        1000
void IRAM_ATTR TimerHandler();




// ----- EEPROM and ShotTime ----------------------------------------------
#define EEPROM_SIZE 8
#define SHOTTIME_ADDR 0
uint32_t shotTime = 0;


// ----- Reed Sensor ------------------------------------------------------
#define D7 (13)
#define PUMP_PIN D7
#define DEBOUNCE_TIME_REED_ON 0
#define DEBOUNCE_TIME_REED_OFF 2000

bool reedContact = false;
uint32_t reedContactTimeClosed = 0;
uint32_t reedContactTimeOpened = 0;



// ----- Push Button ------------------------------------------------------
#define D3 (0)
#define PB_PIN D3
#define DEBOUNCE_TIME 50
#define HOLD_TIME 400
#define PB_ANIMATION_TIME 1000
bool pbPressed = false;
uint32_t pbStoreTime = 0;
uint32_t pbPressedTime = 0;
bool pbStore = false;


// ----- Parsed Machine Data / Serial -------------------------------------
#define SERIAL_TIMEOUT 5000 //in m sec
#define MARAX_DATA_STRLEN 25
const byte numChars = 32;
char receivedChars[numChars];
char field[6][8];
bool newMachineInput = false;
static byte ndx = 0;
char endMarker = '\n';
char rc;

char firstToken[8] = {'C','0','.', '0', '0'};
char priorityMode;
char swVer[8] = {'0','.', '0', '0'}; 
char actSteamTemp[8]= {"000"}; 
char tarSteamTemp[8]= {"000"};
char actHeatExcTemp[8]= {"000"}; 
char boostHeatTime[8]= {"0000"};
bool heatElem = false;
bool machineOn = false;

#define D5 (14)
#define D6 (12)
SoftwareSerial mySerial(D5, D6);


// ----- WIFI ----------------------------------------------------------------
WiFiClient espClient; 
#define WIFI_MAX_INITCNT 10
#define WIFI_MAX_CONNECTCNT 100
#define RSSI_SAMPLECNT 10

uint32_t wifiReconnectDelay[] = {100, 500, 1000, 5000, 10000, 30000, 60000, 300000};
uint32_t prevWifiConnectTime = 0; 
uint8_t wifiInitCnt = 0;
uint8_t wifiConnectCnt = 0;
uint8_t wifiReconnectCnt = 0; 
int rssi = 0;
int rssiAv = 0;
uint8_t rssiCnt = 0;
uint32_t lastWifiTime;

enum rssi
{
  UNAVAILABLE, 
  BAD, 
  GOOD, 
  STRONG
}wifi_rssi=UNAVAILABLE;



// ----- MQTT ----------------------------------------------------------------
// Topics
#define CLIENT "MARAX"
#define will_topic "MaraX/status"
#define online_topic "MaraX/machineOn"
#define sw_topic "MaraX/softwareVersion"
#define actSteam_topic "MaraX/actualSteamTemp"
#define tarSteam_topic "MaraX/targetSteamTemp"
#define actHeatExc_topic "MaraX/actualHeatExchangerTemp"
#define boostHeat_topic "MaraX/boostHeatElementTimer"
#define heatElem_topic "MaraX/heatElementActive"

uint32_t oldMsgTime;
uint16_t mqttMsgCycle = MQTT_DEFAULT_CYCLE;

PubSubClient client(espClient);


// ----- Display --------------------------------------------------------------
Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool displayOn = true;
char outMin[2];

void displayBoostTimer();
