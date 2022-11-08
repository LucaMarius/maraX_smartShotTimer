void sendMQTTMsg()
{
  if(millis()-oldMsgTime > mqttMsgCycle)
  {   
    if(!client.connected())
    {
      reconnectMQTT();
    }

    if(machineOn && newMachineInput)
    {
      mqttMsgCycle = MQTT_WOKRING_CYCLE;
      
  
      client.publish(will_topic, "online", true);
      client.publish(online_topic, "on", true);
      client.publish(sw_topic, String(swVer).c_str(), true);
      client.publish(actSteam_topic, String(actSteamTemp).c_str(), true);
      client.publish(tarSteam_topic, String(tarSteamTemp).c_str(), true);
      client.publish(actHeatExc_topic, String(actHeatExcTemp).c_str(), true);
      client.publish(boostHeat_topic, String(boostHeatTime).c_str(), true);
      client.publish(heatElem_topic, String(heatElem).c_str(), true);
      
      newMachineInput = false;
    }
    else if(!machineOn)
    {
      mqttMsgCycle = MQTT_DEFAULT_CYCLE;
      client.publish(will_topic, "online", true);
      client.publish(online_topic, "off", true);
    }
    oldMsgTime = millis();
  }
}


void reconnectMQTT()
{
  Serial.print("\nAttempting MQTT connection ..."); 
  
  if(client.connect(CLIENT, MQTT_USER, MQTT_PASSWORD, will_topic, 0, true, "offline"))
  {
    Serial.print(" Connected");
  }

  if(!client.connected())
  {
    Serial.print(" Disconnected");
  }
}