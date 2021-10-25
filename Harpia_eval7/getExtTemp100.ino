void updateBME180(){

  ExtTempC = bmp.readTemperature();
  ExtPressPa = bmp.readPressure();

}
