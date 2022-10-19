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
      
      newMachineInput = true;
      machineOn = true;

      
    }
  }

  if (millis() - serialUpdateMillis > 5000) {
    serialUpdateMillis = millis();
    memset(receivedChars, 0, numChars );
    mySerial.write(0x11); // request serial update
    machineOn = false;
  }
}

void parseMachineData()
{
  char * strtokIndex;

  char tempChars[numChars];
  strcpy(tempChars,receivedChars);

  
  strtokIndex = strtok(tempChars, ",");
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
  strcpy(heatElem, strtokIndex);
}
