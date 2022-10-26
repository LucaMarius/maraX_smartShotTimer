void getMachineInput() {
  while (mySerial.available() ) {
    
    serialUpdateMillis = millis();
    rc = mySerial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';
      ndx = 0;
      
      machineOn = true;
      parseMachineData();

      if(DEBUG)
      {
        Serial.print("\nReceived data: "); 
        Serial.print(receivedChars);
      }
    }
  }

  if (millis() - serialUpdateMillis > SERIAL_TIMEOUT) //Serial timeout
  {
    serialUpdateMillis = millis();
    machineOn = false;
  }
}

void parseMachineData()
{
  char * strtokIndex;
  char tempChars[numChars];

  strcpy(tempChars,receivedChars);
  
  //check if received data start with mode "C" (Coffee) or "V" (Vapour)
  if(strlen(tempChars) == MARAX_DATA_STRLEN && (tempChars[0] == 'C' || tempChars[0] == 'V'))
  {
    
    strtokIndex = strtok(tempChars, ",");

    while(strtokIndex != NULL)
    {
      strcpy(swVer, strtokIndex); 

      strtokIndex = strtok(NULL, ",");
      strcpy(actSteamTemp, strtokIndex); 

      strtokIndex = strtok(NULL, ",");
      strcpy(tarSteamTemp, strtokIndex);

      strtokIndex = strtok(NULL, ",");
      strcpy(actHeatExcTemp, strtokIndex); 

      strtokIndex = strtok(NULL, ",");
      strcpy(boostHeatTime, strtokIndex); 

      strtokIndex = strtok(NULL, ",");

      if(!strncmp(strtokIndex, "1", 1))
      {
        heatElem = true;
      }
      else heatElem = false;

      strtokIndex = strtok(NULL, ","); //should return NULL
    }

    newMachineInput = true;

    if(DEBUG)
    {
      Serial.print("\nParsed data:   ");
      Serial.print(swVer);
      Serial.print(",");
      Serial.print(actSteamTemp);
      Serial.print(",");
      Serial.print(tarSteamTemp);
      Serial.print(",");
      Serial.print(actHeatExcTemp);
      Serial.print(",");
      Serial.print(boostHeatTime);
      Serial.print(",");
      Serial.print(heatElem);
    }
  }
}
