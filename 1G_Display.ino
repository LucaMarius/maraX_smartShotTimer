void displayBoostTimer();

void displayHandler() {
  display.clearDisplay();
  if (displayOn) 
  {
    if (timerStarted) 
    {
      displayTimer();
    }
    else if(!timerStarted && millis()-pbStoreTime<=PB_ANIMATION_TIME)
    {
      displayButtonPressed(pbStore);
    }
    else if (!timerStarted && machineOn)
    {
      displayMachineOn();
      displayRSSI();
    }
   else if(!machineOn)
   {
      displayMachineOff();
      displayRSSI();
   } 
  }
  display.display();
}


void displayMachineOn()
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
    if (priorityMode == 'C') 
    {
      display.print("C");
    } 
    else if (priorityMode == 'V') 
    {
      display.print("S");
    } 
    else 
    {
      display.print("X");
    }
  }

  displayBoostTimer(); 

  // draw temperature
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

  // draw steam temperature
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

void displayMachineOff()
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

void displayTimer()
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

void displayBoostTimer()
{
  if (!strcmp(boostHeatTime, "0000")) 
  {
  // not in boost heating mode
  // draw fill circle if heating on
    if (heatElem) 
    {
      display.fillCircle(25, 7, 6, SSD1306_WHITE);
    }
    // draw empty circle if heating off
    else
    {
      display.drawCircle(25, 7, 6, SSD1306_WHITE);
    }
  } 
  else 
  {
    // in boost heating mode
    // draw fill rectangle if heating on
    if (heatElem) 
    {
      display.fillRect(19, 1, 12, 12, SSD1306_WHITE);
    }
    // draw empty rectangle if heating off
    else
    {
      display.drawRect(19, 1, 12, 12, SSD1306_WHITE);
    }
  }
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

void displayRSSI()
{
  switch(wifi_rssi)
  {
    case BAD:
      display.fillCircle(62, 11, 2, SSD1306_WHITE); //weak signal
      break;
    case GOOD:
      display.fillCircle(62, 11, 2, SSD1306_WHITE); //weak signal
      display.drawLine(58, 7, 62, 5, SSD1306_WHITE); // good signal left
      display.drawLine(62, 5, 66, 7, SSD1306_WHITE); // good signal right
      break;
    case STRONG:
      display.fillCircle(62, 11, 2, SSD1306_WHITE); //weak signal
      display.drawLine(58, 7, 62, 5, SSD1306_WHITE); // good signal left
      display.drawLine(62, 5, 66, 7, SSD1306_WHITE); // good signal right

      display.drawLine(56, 3, 62, 1, SSD1306_WHITE); // strong signal left
      display.drawLine(62, 1, 68, 3, SSD1306_WHITE); // strong signal right
      break; 
    default:
      //display.drawCircle(62, 11, 2, SSD1306_WHITE); //no signal
      display.fillCircle(62, 11, 2, SSD1306_WHITE); //weak signal
      display.drawLine(58, 7, 62, 5, SSD1306_WHITE); // good signal left
      display.drawLine(62, 5, 66, 7, SSD1306_WHITE); // good signal right

      display.drawLine(56, 3, 62, 1, SSD1306_WHITE); // strong signal left
      display.drawLine(62, 1, 68, 3, SSD1306_WHITE); // strong signal right

      display.drawLine(56, 13, 68, 0, SSD1306_WHITE); // strong signal right
      break;
 
  }  
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