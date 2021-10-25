void showScreenA()
{
  char buff[10];
  
            String thisString = String(cardUsedSpace);
            String thisString2 = String(error_counter);
            
            display.displayOn();
            display.setFont(ArialMT_Plain_10);
              display.drawString(0, 0, currentFullDate);        
              //display.drawString(0, 13, wifiIP);//display.drawString(0, 13, "Wifi IP:   ");display.drawString(50, 13, wifiIP);
              display.drawString(0, 13, "RSSI       :     ");display.drawString(70, 13, dtostrf(wifiRSSI, 2, 0, buff)); 
              display.drawString(0, 26, "SDcard (% ):     ");display.drawString(70, 26, thisString);  
              display.drawString(0, 39, fileName);
              display.drawString(0, 52, "ErrorCnt(100):   ");display.drawString(70, 52, thisString2); 
            //display.setFont(ArialMT_Plain_24);
              //  display.drawString(75, 13, dtostrf(ExtTempC, 2, 1, buff)); 
            display.display();         
}
