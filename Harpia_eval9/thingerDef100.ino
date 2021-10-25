void ThingerDefinitions(){
    thing["data"] >> [](pson& out){
    // Add the values and the corresponding code
    out["swVersion"] = ino;
    out["RSSI"] = wifiRSSI;
    out["lastUpdate"] = currentFullDate;

    out["ExtTemp"] = ExtTempC;
    out["ExtPress"] = ExtPressPa;
    out["LastPicture"] = fileName;
  };
}
