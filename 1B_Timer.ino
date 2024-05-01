uint8_t getTimer() {
  
  if (timerStarted) 
  {
    timerCount = (millis() - timerStartMillis ) / 1000;
  } 

  if (timerCount > 99) 
  {
    return 99;
  }
  
  return timerCount;
}