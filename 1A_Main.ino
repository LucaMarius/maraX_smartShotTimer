void setup() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(SHOTTIME_ADDR, shotTime);
  
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(200);

  if(DEBUG)
  {
    Serial.print("\nDEBUG INFO -------------------------");
    Serial.print("\n\nONLINE MODE: "); 
    if(ONLINEMODE)
    {
      Serial.print("active ------");
      
      Serial.print("\nWiFi Credentials:");
      Serial.print("\nSSID: "); Serial.print(ssid); 
      Serial.print("\nPassword: "); Serial.print(wpa2);   
    }
    else Serial.print("Not active, Working in local mode");

    Serial.print("\n\nMQTT MODE: "); 
    if(ONLINEMODE && MQTTMODE)
    {
      Serial.print("active ------");
      
      Serial.print("\nMQTT Credentials:");
      Serial.print("\nServer: "); Serial.print(MQTT_SERVER); 
      Serial.print("\nPort: "); Serial.print(MQTT_PORT); 
      Serial.print("\nUser: "); Serial.print(MQTT_USER);  
      Serial.print("\nPassword: "); Serial.print(MQTT_PASSWORD);  
    }
    else Serial.print("Not active");
    
    Serial.print("\n\nDEBUG INFO END ---------------------");
        
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);   
  }

  if(ONLINEMODE)
  {
    initWifi();

    if(MQTTMODE)
    {
      client.setServer(MQTT_SERVER, MQTT_PORT);
    }   
  }
  
  pinMode(PUMP_PIN, INPUT_PULLUP);
  pinMode(PB_PIN, INPUT_PULLUP);

  

  memset(receivedChars, 0, numChars );

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();

  displayMode(ONLINEMODE);
  t.every(100, displayHandler);
  
  mySerial.write(0x11);

  // Interval in microsecs
  //ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 1000, TimerHandler);
}

uint32_t startTime = 0;

uint32_t startTime_pump = 0;
uint32_t startTime_button = 0;
uint32_t startTime_getData = 0;
uint32_t startTime_wifiHandler = 0;
uint32_t startTime_mqtt = 0;

int32_t time_pump = 0;
int32_t time_button = 0;
int32_t time_getData = 0;
int32_t time_wifiHandler = 0;
int32_t time_mqtt = 0;



void loop() 
{
  t.update();
  
  startTime_pump = millis();
  detectPumpChanges();
  time_pump = millis()-startTime_pump;
  if(machineOn) client.publish("MaraX/Debug/pumpTime", String(time_pump).c_str(), true);
  
  if(time_pump >=5) 
  {
    Serial.print("\n------Time Pump Handler: "); 
    Serial.print(time_pump);
  }

  startTime_button = millis();
  detectButtonChanges();
  time_button = millis()-startTime_button;
  if(machineOn) client.publish("MaraX/Debug/buttonTime", String(time_button).c_str(), true);
  if(time_button >=5) 
  {
    Serial.print("\n------Time Button Handler: "); 
    Serial.print(time_button);
  }


  if(millis()-startTime >= 500)
  {
    startTime_getData = millis();
    getMachineInput();
    time_getData = millis()-startTime_getData;
    if(machineOn) client.publish("MaraX/Debug/dataTime", String(time_getData).c_str(), true);
    if(time_getData >=5)
    {
      Serial.print("\n------Time Machinedata Handler: "); 
      Serial.print(time_getData);
    }

    startTime = millis();
  }
  

  if(ONLINEMODE)
  {
    startTime_wifiHandler = millis();
    wifiHandler();
    time_wifiHandler = millis()-startTime_wifiHandler;
    if(machineOn) client.publish("MaraX/Debug/wifiTime", String(time_wifiHandler).c_str(), true);
    if(time_wifiHandler >= 5)
    {
      Serial.print("\n------Time WIFI Handler: "); 
      Serial.print(time_wifiHandler);
    } 


    if(WiFi.status() == WL_CONNECTED && MQTTMODE)
    {
      startTime_mqtt = millis();
      sendMQTTMsg(); 
      client.loop();
      time_mqtt = millis()-startTime_mqtt;
      if(machineOn) client.publish("MaraX/Debug/mqttTime", String(time_mqtt).c_str(), true);
      if(time_mqtt >= 5) 
      {
        Serial.print("\n------Time MQTT Handler: "); 
        Serial.print(time_mqtt);
      }
    } 

    
  }
  

}
