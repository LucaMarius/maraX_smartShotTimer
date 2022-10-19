void detectPumpChanges() 
{
  if (!timerStarted && !digitalRead(PUMP_PIN)) {
    timerStartMillis = millis();
    timerStarted = true;
    displayOn = true;

    if(DEBUG)
    {
      Serial.print("\nStart pump");
    } 
  }
  if (timerStarted && digitalRead(PUMP_PIN)) {
    if (timerStopMillis == 0) {
      timerStopMillis = millis();
    }
    if (millis() - timerStopMillis > 500) {
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
  } 
  else 
  {
    timerStopMillis = 0;
  }
  
  if (!timerStarted && displayOn && timerDisplayOffMillis >= 0 && (millis() - timerDisplayOffMillis > 1000 * 60 * 60)) {
    timerDisplayOffMillis = 0;
    timerCount = 0;
    prevTimerCount = 0;
    displayOn = false;
    

    if(DEBUG)
      {
        Serial.print("\nSleep");
      } 
  }
 
  if(DEBUG)
  {
    digitalWrite(LED_BUILTIN, digitalRead(PUMP_PIN));
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
