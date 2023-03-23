void getMachineInput() {
  while (mySerial.available()>0) {
    
    
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
    
    uint8_t fieldCnt = 0;
    uint8_t maxFieldCnt = 5;
    strtokIndex = strtok(tempChars, ",");

    while(strtokIndex != NULL && fieldCnt<=maxFieldCnt)
    {
      strcpy(field[fieldCnt], strtokIndex);
      strtokIndex = strtok(NULL, ",");

      fieldCnt++;
    }

    if(fieldCnt == 6)
    {
      strcpy(firstToken, field[0]); //store first token which contains priority mode and sw version
      priorityMode = firstToken[0]; 
      strncpy(swVer, &firstToken[0]+1, 4);

      strcpy(actSteamTemp, field[1]); 
      strcpy(tarSteamTemp, field[2]);
      strcpy(actHeatExcTemp, field[3]); 
      strcpy(boostHeatTime, field[4]); 

      if(!strncmp(field[5], "1", 1))
      {
        heatElem = true;
      }
      else heatElem = false;

    }

    if(DEBUG && false)
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
