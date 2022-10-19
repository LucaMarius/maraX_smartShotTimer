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
      parseMachineData();
  
      client.publish(will_topic, "online", true);
      client.publish(online_topic, String(machineOn).c_str(), true);
      client.publish(sw_topic, String(swVer).c_str(), true);
      client.publish(actSteam_topic, String(actSteamTemp).c_str(), true);
      client.publish(tarSteam_topic, String(tarSteamTemp).c_str(), true);
      client.publish(actHeatExc_topic, String(actHeatExcTemp).c_str(), true);
      client.publish(boostHeat_topic, String(boostHeatTime).c_str(), true);
      client.publish(heatElem_topic, String(heatElem).c_str(), true);
      
      newMachineInput = false;
    }
    else
    {
      mqttMsgCycle = MQTT_DEFAULT_CYCLE;
      client.publish(will_topic, "online", true);
      client.publish(online_topic, String(machineOn).c_str(), true);
      client.publish(actSteam_topic, "---", true);
      client.publish(tarSteam_topic, "---", true);
      client.publish(actHeatExc_topic, "---", true);
      client.publish(boostHeat_topic, "---", true);
      client.publish(heatElem_topic, "---", true);
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