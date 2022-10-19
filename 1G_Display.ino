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
      if (receivedChars[0] ) {
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
      if (String(receivedChars).substring(18, 22) == "0000") {
        // not in boost heating mode
        // draw fill circle if heating on
        if (String(receivedChars[23]) == "1") {
          display.fillCircle(45, 7, 6, SSD1306_WHITE);
        }
        // draw empty circle if heating off
        if (String(receivedChars[23]) == "0") {
          display.drawCircle(45, 7, 6, SSD1306_WHITE);
        }
      } else {
        // in boost heating mode
        // draw fill rectangle if heating on
        if (String(receivedChars[23]) == "1") {
          display.fillRect(39, 1, 12, 12, SSD1306_WHITE);
        }
        // draw empty rectangle if heating off
        if (String(receivedChars[23]) == "0") {
          display.drawRect(39, 1, 12, 12, SSD1306_WHITE);
        }
      }
      // draw temperature
      if (receivedChars[14] && receivedChars[15] && receivedChars[16]) {
        display.setTextSize(3);
        display.setCursor(1, 20);
        if (String(receivedChars[14]) != "0") {
          display.print(String(receivedChars[14]));
        }
        display.print(String(receivedChars[15]));
        display.print(String(receivedChars[16]));
        display.print((char)247);
        if (String(receivedChars[14]) == "0") {
          display.print("C");
        }
      }
      // draw steam temperature
      if (receivedChars[6] && receivedChars[7] && receivedChars[8]) {
        display.setTextSize(2);
        display.setCursor(1, 48);
        if (String(receivedChars[6]) != "0") {
          display.print(String(receivedChars[6]));
        }
        display.print(String(receivedChars[7]));
        display.print(String(receivedChars[8]));
        display.print((char)247);
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
