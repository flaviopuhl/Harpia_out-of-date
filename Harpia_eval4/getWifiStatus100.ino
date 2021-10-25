void getWifiStatus(){

    wifiSSID = WiFi.SSID();                   // Must be declared as global
    wifiIP = ipToString(WiFi.localIP());      // Must be declared as global
    wifiRSSI = WiFi.RSSI();                   // Must be declared as global
    
    Serial.println("");
    Serial.println("############### Current WIFI ###############\n");
    Serial.print("SSID             : "); Serial.println(wifiSSID);
    Serial.print("IP               : "); Serial.println(wifiIP);
    Serial.print("RSSI             : "); Serial.println(wifiRSSI);
    Serial.print("Status           : "); Serial.println(wl_status_to_string(WiFi.status()));
    Serial.println();

    if (WiFi.status() != WL_CONNECTED || WiFi.localIP() == IPAddress(0, 0, 0, 0)){
      error_counter++;
    }

}    

    const char* wl_status_to_string(wl_status_t status) {
      switch (status) {
        case WL_NO_SHIELD: return "WL_NO_SHIELD";
        case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
        case WL_CONNECTED: return "WL_CONNECTED";
        case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED: return "WL_DISCONNECTED";
      }
    }



    String ipToString(IPAddress ip){
              String s="";
              for (int i=0; i<4; i++)
                s += i  ? "." + String(ip[i]) : String(ip[i]);
              return s;
    }
