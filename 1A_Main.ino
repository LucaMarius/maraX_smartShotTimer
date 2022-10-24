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
  t.every(100, updateDisplay);
  
  mySerial.write(0x11);
}

uint32_t startTimet = 0;

void loop() {
  t.update();
  
  detectPumpChanges();
  detectButtonChanges();

  if(millis()-startTimet >= 1000)
  {
    getMachineInput();
    startTimet = millis();
  }
  

  if(ONLINEMODE)
  {
    wifiHandler();
    if(WiFi.status() == WL_CONNECTED && MQTTMODE)
    {
      sendMQTTMsg(); 
    } 
  }

}
