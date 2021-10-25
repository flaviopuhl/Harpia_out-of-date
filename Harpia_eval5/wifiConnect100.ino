void wifiConnect(){
 
Serial.println("");
Serial.printf("Connecting to %s ", ssid);   //connect to WiFi
WiFi.begin(ssid, password);

  int wificount=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
      if(wificount>=30){ ESP.restart(); } else { wificount++; }

    }
    Serial.println(" CONNECTED");
}
