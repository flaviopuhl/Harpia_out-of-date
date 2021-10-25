/*********
 * HARPIA
  ESP32 CAM save pic to SD ................................................. [DONE]
  eval 1: Connect to internet .............................................. [DONE]
  eval 2: Move image code to a subroutine .................................. [DONE]
  eval 3: Add NTP, assign to file name, remove EEPROM feat .................
  eval 4: Add firebase storage feat. .......................................
  eval 5: Add display feat. ................................................
  eval 6: Add BME280 feat. .................................................
  eval 7: Add Thinger ......................................................
  
*********/

/*************************************************************************************
 * Libraries
*************************************************************************************/
#define _DISABLE_TLS_                                         //Thinger

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>            // read and write from flash memory

#include <WiFi.h>              // Wifi for ESP32
#include <ThingerESP32.h>      // Thinger
#include "time.h"

#include "SSD1306.h"

#include <Wire.h>
#include <Adafruit_BMP085.h>

/*************************************************************************************
 * Declaring prototypes (needed when using Arduino IDE tabs)
*************************************************************************************/
void wifiConnect();  
void getWifiStatus();
void takeImage();
void DateandTimerequest();

/*************************************************************************************
 * Global definitions
*************************************************************************************/

// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

int pictureNumber = 0;

const char *ssid     = "CasaDoTheodoro";                      // Wifi
const char *password = "09012011";
                                            
String wifiSSID;                                              // getWifiStatus()
String wifiIP;                                                // getWifiStatus()
int wifiRSSI;                                                 // getWifiStatus()

String currentTime;                                           // NPTrequest()
String currentDate;                                           // NPTrequest()
String currentFullDate;                                       // NPTrequest()


/*************************************************************************************
 * Setup
*************************************************************************************/

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);
    delay(1000);                                  // delay to stabilize the serial comm

  configTime(-10800, 0, "pool.ntp.org");                      // NPT


  wifiConnect();                                  // Connect to wifi network
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  
  // Init Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  //Serial.println("Starting SD Card");
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }
    
  
}

/*************************************************************************************
 * Setup
*************************************************************************************/

void loop() {

  getWifiStatus();                      // refresh wifi network status 
  DateandTimerequest();                 // get date&time from Network
  takeImage();                          // take image from camera
 
    delay(10000);
  
}
