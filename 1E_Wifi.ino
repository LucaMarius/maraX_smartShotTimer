void initWifi()
{
  WiFi.begin(ssid, wpa2);
  Serial.print("\n\nTrying to connect to WiFi");
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    wifiInitCnt++;
    Serial.print("."); 

    if(wifiInitCnt > WIFI_MAX_INITCNT)
    {
      wifiInitCnt = 0;
      break;
    } 
  }

  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print("\n"); 
    Serial.print("\n---WIFI CONNECTED---"); 
    Serial.print("\nIPV4: "); 
    Serial.print(WiFi.localIP());
  }
  else
  {
    Serial.print("\n"); 
    Serial.print("\n---WIFI ");
    Serial.print(" DISCONNECTED---");     
  }
}


void wifiHandler()
{
  if(WiFi.status() == WL_CONNECTED) //if connected via wifi try to send mqtt msg
  {
    wifiReconnectCnt = 0;
  }
  else if(!timerStarted && WiFi.status() != WL_CONNECTED && (millis()-prevWifiConnectTime > wifiReconnectDelay[wifiReconnectCnt])) //if not connected try again later, reconnect delay time inceases with a failed reconnect
  {
    prevWifiConnectTime = millis();
    initWifi();
    wifiReconnectCnt++;

    if(wifiReconnectCnt > sizeof(wifiReconnectDelay)/sizeof(wifiReconnectDelay[0])-1)
    {
      wifiReconnectCnt = sizeof(wifiReconnectDelay)/sizeof(wifiReconnectDelay[0])-1;
    }

    Serial.print("\nReconnect to Wifi - Cnt: ");
    Serial.print(wifiReconnectCnt);
    Serial.print(" - Next Timeout: ");
    Serial.print(wifiReconnectDelay[wifiReconnectCnt]);   
    Serial.print(" - prev Time: ");
    Serial.print(prevWifiConnectTime);  
  } 
}
