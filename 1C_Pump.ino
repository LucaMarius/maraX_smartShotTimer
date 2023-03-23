void detectPumpChanges() 
{
  if (!digitalRead(PUMP_PIN) && !reedContact) { //reed contac closed
    reedContact = true;
    reedContactTimeClosed = millis();
    if(DEBUG)
    {
      Serial.print("\nSwitch closed");
    }
  }

  else if(!digitalRead(PUMP_PIN) && reedContact && (millis()-reedContactTimeClosed >= DEBOUNCE_TIME_REED_ON) && !timerStarted) //reed contac closed for >= DEBOUNCE_TIME
  {
    timerStarted = true;
    timerStartMillis = millis();
    displayOn = true;

    if(DEBUG)
    {
      Serial.print("\nStart pump");
    }
  }

  else if(digitalRead(PUMP_PIN) && reedContact) //reed contac opened
  {
    reedContact = false;
    reedContactTimeOpened = millis();

    if(DEBUG)
    {
      Serial.print("\nSwitch opened");
    } 
  }

  else if(digitalRead(PUMP_PIN) && !reedContact && (millis()-reedContactTimeOpened >= DEBOUNCE_TIME_REED_OFF) && timerStarted) //reed contac opened for >= DEBOUNCE_TIME
  {
    timerStarted = false;

    timerStopMillis = 0;
    timerDisplayOffMillis = millis();
    display.invertDisplay(false);

    if(pbStore && shotTime != getTimer()) //if storage button was pressed -> save new shotime to EEPROM
    {
      shotTime = getTimer();
      EEPROM.put(SHOTTIME_ADDR, shotTime);
      EEPROM.commit();

      pbStore = false;
      
      if(DEBUG)
      {
        uint8_t eepromTime=10;
        EEPROM.get(SHOTTIME_ADDR, eepromTime);
        Serial.print("\nStored ShotTime: "); 
        Serial.print(eepromTime);
      }
    }

    if(DEBUG)
    {
      Serial.print("\nStop pump");
    } 
  }

  if (!machineOn && !timerStarted && displayOn && timerDisplayOffMillis >= 0 && (millis() - timerDisplayOffMillis > SLEEPTIME)) {
    timerDisplayOffMillis = 0;
    timerCount = 0;
    prevTimerCount = 0;
    displayOn = false;
    

    if(DEBUG)
      {
        Serial.print("\nSleep");
      } 
  }

}

void detectButtonChanges()
{ 
  if(!digitalRead(PB_PIN) && pbPressed == false) //falling flag
  {
    pbPressed = true;
    pbPressedTime = millis();
  }

  else if(!digitalRead(PB_PIN) && pbPressed && (millis()-pbPressedTime >= DEBOUNCE_TIME) && (millis()-pbPressedTime < HOLD_TIME) && !pbStore) //pressed, prevent toggle and single store
  {
    pbStoreTime = millis();
    pbStore = true;

    if(DEBUG)
    {
      Serial.print("\nPB Pushed: "); Serial.print(pbStore);
    }
  }

  else if(!digitalRead(PB_PIN) && pbStore && (millis()-pbPressedTime >= HOLD_TIME)) //long pressed, release pb store
  {
    pbStoreTime = millis();
    pbStore = false;

    if(DEBUG)
    {
      Serial.print("\nPB Released: "); Serial.print(pbStore);
    }
  }

  else if(digitalRead(PB_PIN)) //rising flag
  {
    pbPressed = false;
  }
}
