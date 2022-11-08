void getMachineInput() {
  while (mySerial.available() ) {
    
    
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
      
      parseMachineData();

      if(DEBUG)
      {
        Serial.print("\nReceived data: "); 
        Serial.print(receivedChars);
      }
    }
  }

  if (machineOn && (millis() - serialUpdateMillis > SERIAL_TIMEOUT)) //Serial timeout
  {
    serialUpdateMillis = millis();
    machineOn = false;
    timerDisplayOffMillis = millis();
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
    serialUpdateMillis = millis();
    newMachineInput = true;
    machineOn = true;
    displayOn = true;
    
    strtokIndex = strtok(tempChars, ",");

    while(strtokIndex != NULL)
    {
      strcpy(firstToken, strtokIndex); //store first token which contains priority mode and sw version

      priorityMode = firstToken[0]; 
      strncpy(swVer, strtokIndex+1, 4);

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

    if(DEBUG)
    {
      Serial.print("\nParsed data:   ");
      Serial.print(priorityMode);
      Serial.print("");
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
