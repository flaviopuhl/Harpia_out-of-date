void takeImage(){

 Serial.println("################# CAM start #################\n");
   
  camera_fb_t * fb = NULL;
  
  // Take Picture with Camera
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  
  // initialize EEPROM with predefined size
  //EEPROM.begin(EEPROM_SIZE);
  //pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  //String path = "/picture" + String(pictureNumber) +".jpg";
  String path = "/Harpia_picture" + currentFullDate +".jpg";  // file name with date&Time
  String pathStatic = "/Harpia_picture_staticRef.jpg";        // file name with fixed name for Thinger

  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  Serial.printf("Picture file name: %s\n", pathStatic.c_str());

  // Save image with date&time info
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file in writing mode");
    } 
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.printf("Saved file to path: %s\n", path.c_str());
      //EEPROM.write(0, pictureNumber);
      //EEPROM.commit();
    }
    file.close();

  // Save image with fixed name for Thinger
    File fileStatic = fs.open(pathStatic.c_str(), FILE_WRITE);
    if(!fileStatic){
      Serial.println("Failed to open file (Static) in writing mode");
    } 
    else {
      fileStatic.write(fb->buf, fb->len); // payload (image), payload length
      Serial.printf("Saved file (Static) to path: %s\n", pathStatic.c_str());
      //EEPROM.write(0, pictureNumber);
      //EEPROM.commit();
    }
    fileStatic.close();


  
  esp_camera_fb_return(fb); 
  
  
  
  
  
  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  //pinMode(4, OUTPUT);                   // Those lines must be commented to allow
  //digitalWrite(4, LOW);                 //  the SD card pin to be released
  //rtc_gpio_hold_en(GPIO_NUM_4);         // If not commented, SD card file saving on
                                          //  2nd loop will result in critical fault

 delay(100);
 Serial.println("Pic Taken");
}
