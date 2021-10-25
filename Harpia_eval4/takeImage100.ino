void takeImage(){

 Serial.println("################# CAM start #################\n");
   
  camera_fb_t * fb = NULL;
  
  // Take Picture with Camera
  fb = esp_camera_fb_get();  
  if(!fb) {
    Serial.println("Camera capture failed");
      error_counter++;
    return;
  }
  
  // initialize EEPROM with predefined size
  //EEPROM.begin(EEPROM_SIZE);
  //pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  //String path = "/picture" + String(pictureNumber) +".jpg";
  String path = "/Harpia_picture " + currentFullDate +".jpg";  // file name with date&Time
  String pathStatic = "/Harpia_pic.jpg";                       // file name with fixed name for Thinger

  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  Serial.printf("Picture file name: %s\n", pathStatic.c_str());

  // Save image with date&time info
    File file = fs.open(path.c_str(), FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file in writing mode");
      error_counter++;
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
      error_counter++;
    } 
    else {
      fileStatic.write(fb->buf, fb->len); // payload (image), payload length
      Serial.printf("Saved file (Static) to path: %s\n", pathStatic.c_str());
      //EEPROM.write(0, pictureNumber);
      //EEPROM.commit();
    }
    fileStatic.close();

if (Firebase.ready())
    {
        taskCompleted = true;
    
        Serial.print("Upload file test to Firebase... ");

        //MIME type should be valid to avoid the download problem.
        //The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
        if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "/Harpia_pic.jpg" /* path to local file */, mem_storage_type_sd /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, "Harpia_pic.jpg" /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */)) 
            Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
        else
            Serial.println(fbdo.errorReason());
            error_counter++;
    }
  
  esp_camera_fb_return(fb); 
  
  
  
  
  
  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  //pinMode(4, OUTPUT);                   // Those lines must be commented to allow
  //digitalWrite(4, LOW);                 //  the SD card pin to be released
  //rtc_gpio_hold_en(GPIO_NUM_4);         // If not commented, SD card file saving on
                                          //  2nd loop will result in critical fault

 delay(100);

}
