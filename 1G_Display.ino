void updateDisplay() {
  display.clearDisplay();
  if (displayOn) 
  {
    if (timerStarted) 
    {
      if(!pbStore)
      {
      display.setTextSize(7);
      display.setCursor(25, 8);
      sprintf(outMin, "%02u", getTimer());
      display.print(outMin);
      }

      else if(pbStore && millis()-pbStoreTime<=PB_ANIMATION_TIME)
      {
        displayButtonPressed(pbStore);
      }

      else if(pbStore)
      {
        // draw line
        display.drawLine(1, 0, 1, 63, SSD1306_WHITE);
        display.drawLine(5, 0, 5, 63, SSD1306_WHITE);
        display.drawLine(122, 0, 122, 63, SSD1306_WHITE);
        display.drawLine(127, 0, 127, 63, SSD1306_WHITE);
        
        display.setTextSize(7);
        display.setCursor(25, 8);
        sprintf(outMin, "%02u", getTimer());
        display.print(outMin);
      }
    }
    else if(!timerStarted && millis()-pbStoreTime<=PB_ANIMATION_TIME)
    {
      displayButtonPressed(pbStore);
    }

    else if (!timerStarted && machineOn)
    {
      // draw line
      display.drawLine(74, 0, 74, 63, SSD1306_WHITE);
      // draw last shot time
      display.setTextSize(4);
      display.setCursor(display.width() / 2 - 1 + 17, 20);
      display.print(EEPROM.read(SHOTTIME_ADDR));
      // draw machine state C/S
      if (receivedChars[0]) {
        display.setTextSize(2);
        display.setCursor(1, 1);
        if (String(receivedChars[0]) == "C") {
          display.print("C");
        } else if (String(receivedChars[0]) == "V") {
          display.print("S");
        } else {
          display.print("X");
        }
      }
      if (!strcmp(boostHeatTime, "0000")) 
      {
        // not in boost heating mode
        // draw fill circle if heating on
        if (heatElem) 
        {
          display.fillCircle(45, 7, 6, SSD1306_WHITE);
        }
        // draw empty circle if heating off
        else
        {
          display.drawCircle(45, 7, 6, SSD1306_WHITE);
        }
      } 
      else 
      {
        // in boost heating mode
        // draw fill rectangle if heating on
        if (heatElem) 
        {
          display.fillRect(39, 1, 12, 12, SSD1306_WHITE);
        }
        // draw empty rectangle if heating off
        else
        {
          display.drawRect(39, 1, 12, 12, SSD1306_WHITE);
        }
      }
      // draw temperature
      if (1)//receivedChars[14] && receivedChars[15] && receivedChars[16]) 
      {
        if (String(actHeatExcTemp[0]) != "0") {
          display.setTextSize(2);
          display.setCursor(1, 25);
          display.print(String(actHeatExcTemp[0]));
        }
        else
        {
          display.setTextSize(3);
          display.setCursor(1, 20);
        }
        display.print(String(actHeatExcTemp[1]));
        display.print(String(actHeatExcTemp[2]));
        display.print((char)247); //prints °
        display.print("C");
      }
      // draw steam temperature
      if (1) //receivedChars[6] && receivedChars[7] && receivedChars[8]) 
      {
        display.setTextSize(2);
        display.setCursor(1, 48);
        if (String(actSteamTemp[0]) != "0") 
        {
          display.print(String(actSteamTemp[0]));
        }
        display.print(String(actSteamTemp[1]));
        display.print(String(actSteamTemp[2]));
        display.print((char)247); //prints °
        display.print("C");
      }
    }
   else if(!machineOn)
   {
      //draw machine off
      display.setTextSize(2);
      display.setCursor(18, 25);
      display.print("off");
      
      // draw line
      display.drawLine(74, 0, 74, 63, SSD1306_WHITE);
      // draw last shot time
      display.setTextSize(4);
      display.setCursor(display.width() / 2 - 1 + 17, 20);
      display.print(EEPROM.read(SHOTTIME_ADDR));
   }
    
  }
  display.display();
}


void displayButtonPressed(bool pressedReleased) //true in case pressed, false in case released
{
  if(pressedReleased)
  {
    // draw line
    display.drawLine(1, 0, 1, 63, SSD1306_WHITE);
    display.drawLine(5, 0, 5, 63, SSD1306_WHITE);
    display.drawLine(122, 0, 122, 63, SSD1306_WHITE);
    display.drawLine(127, 0, 127, 63, SSD1306_WHITE);
    
    //draw store time
    display.setTextSize(2);
    display.setCursor(22, 12);
    display.print("storing"); 
    display.setCursor(9, 32);
    display.print("shot time");  
  }
  else
  {
    // draw line
    display.drawLine(1, 0, 1, 63, SSD1306_WHITE);
    display.drawLine(5, 0, 5, 63, SSD1306_WHITE);
    display.drawLine(122, 0, 122, 63, SSD1306_WHITE);
    display.drawLine(127, 0, 127, 63, SSD1306_WHITE);
    
    //draw store time
    display.setTextSize(2);
    display.setCursor(22, 12);
    display.print("storing"); 
    display.setCursor(15, 32);
    display.print("released");  
  }
  display.display(); 
}

void displayMode(bool mode) //true in case online, false in case local mode
{
  if(mode)
  {
    //draw store time
    display.setTextSize(2);
    display.setCursor(26, 12);
    display.print("online"); 
    display.setCursor(36, 32);
    display.print("mode");  
  }
  else
  {
    //draw store time
    display.setTextSize(2);
    display.setCursor(30, 12);
    display.print("local"); 
    display.setCursor(36, 32);
    display.print("mode");  
  }
  display.display();
  delay(3000);
}
