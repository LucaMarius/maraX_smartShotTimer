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
    wifiConnectCnt++; 

    if(wifiConnectCnt > WIFI_MAX_CONNECTCNT)
    {
      wifiConnectCnt = WIFI_MAX_CONNECTCNT;
    }


    if(wifiConnectCnt >= WIFI_MAX_CONNECTCNT)
    {
      int rssiTmp = rssiAvCalc(WiFi.RSSI());

      if(rssiTmp)
      {
        rssi = rssiTmp;
        if(rssi < -60)
        {
          wifi_rssi = STRONG;
        }
        else if(-70 > rssi >= -60)
        {
          wifi_rssi = GOOD;
        }
        else if(rssi >= -70)
        {
          wifi_rssi = BAD;
        }
    }
    
    if(DEBUG && (millis()-lastWifiTime > 5000))
    {
      Serial.print("\n---WIFI CONNECTED---"); 
      Serial.print("\nIPV4: ");
      Serial.print(WiFi.localIP());
      Serial.print("\nRSSI: ");
      Serial.print(rssi);
      Serial.print("dBm");
      Serial.print("\nenum: ");
      Serial.print(wifi_rssi);
      lastWifiTime = millis();
    }

    }


  }
  else if(!timerStarted && WiFi.status() != WL_CONNECTED && (millis()-prevWifiConnectTime > wifiReconnectDelay[wifiReconnectCnt])) //if not connected try again later, reconnect delay time inceases with a failed reconnect
  {
    prevWifiConnectTime = millis();
    initWifi();
    wifi_rssi = UNAVAILABLE;
    wifiConnectCnt = 0;
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

int rssiAvCalc(int rssi)
{
  if(rssiCnt <= RSSI_SAMPLECNT)
  {
    rssiAv += rssi;
    rssiCnt++;
    return 0;
  }
  else
  {
    rssiAv = rssiAv / rssiCnt; 
    rssiCnt = 0;
    return rssiAv;
  }
}
