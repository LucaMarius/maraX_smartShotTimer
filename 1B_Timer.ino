uint8_t getTimer() {
  
  if (timerStarted) 
  {
    timerCount = (millis() - timerStartMillis ) / 1000;
    if (timerCount >= 10) 
    {
      prevTimerCount = timerCount;
    }
  } 
  else 
  {
    timerCount = prevTimerCount;
  }
  
  if (timerCount > 99) 
  {
    return 99;
  }
  
  return timerCount;
}
