/*********
 * HARPIA
  ESP32 CAM save pic to SD ................................................. [DONE]
  eval 1: Connect to internet .............................................. [DONE]
  eval 2: Move image code to a subroutine .................................. [DONE]
  eval 3: Add NTP, assign to file name, remove EEPROM feat ................. [DONE]
  eval 4: Add firebase storage feat. ....................................... [DONE]
  eval 5: SD Card info + sw info ........................................... [DONE]
  eval 6: Add display feat. ................................................ [DONE]
  eval 7: Add BME280 feat. ................................................. [DONE]
  eval 8: Add Thinger ...................................................... [DONE]
  eval 9: Add scheduler .................................................... [DONE]
  eval 10: Replace scheduler by RTOS .......................................
  eavl 11: Add Crypto Mining ...............................................
  
*********/

/*************************************************************************************
 * Libraries
*************************************************************************************/
#define _DISABLE_TLS_                                       //Thinger

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                                             // SD Card ESP32
#include "SD_MMC.h"                                         // SD Card ESP32
#include "soc/soc.h"                                        // Disable brownout problems
#include "soc/rtc_cntl_reg.h"                               // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>                                         // read and write from flash memory

#include <WiFi.h>                                           // Wifi for ESP32
#include <ThingerESP32.h>                                   // Thinger
#include "time.h"

#include "SSD1306.h"                                        // OLED display

#include <Wire.h>                                           // BMP180
#include <Adafruit_BMP085.h>                                // BMP180

#include <Firebase_ESP_Client.h>                            //Firebase
#include "addons/TokenHelper.h"                             // Firebase Provide the token generation process info.

#include <TaskScheduler.h>                                  // Scheduler

/*************************************************************************************
 * Declaring prototypes (needed when using Arduino IDE tabs)
*************************************************************************************/

void wifiConnect();  
void getWifiStatus();
void takeImage();
void DateandTimerequest();
void CardHealth();
void getSWversion();
void showScreenA();
void updateBME180();
void ThingerDefinitions();
void screenLoop();

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


#define API_KEY "AIzaSyCwabnsX5WNClPa18AYMSSyoaRFIXI3rmk"     // Firebase: Define the API Key 
#define USER_EMAIL "flaviopuhljr@gmail.com"                   // Firebase: Define the user Email 
#define USER_PASSWORD "Lanterna@01"                           // Firebase: Define password 
#define STORAGE_BUCKET_ID "harpia-73a7a.appspot.com"          // Firebase: Define the Firebase storage bucket ID e.g bucket-name.appspot.com 

FirebaseData fbdo;                                            // Firebase: Define Firebase Data object
FirebaseAuth auth;                                            // Firebase
FirebaseConfig config;                                        // Firebase

bool taskCompleted = false;                                   // Firebase

int error_counter = 0;                                        // General error counter

unsigned long previousMillis_MainLoop = 0;                    // Main loop timer
unsigned long previousMillis_SecLoop = 0;                     // Secondary loop timer
unsigned long previousMillis_screenLoop = 0;                  // Screen loop timer

String ino = __FILE__;                                        // getSWversion()

uint32_t cardTotalBytes;                                      // SD card health
uint32_t cardUsedBytes;                                       // SD card health
uint32_t cardUsedSpace;                                       // SD card health    

SSD1306 display(0x3c, 1, 3);                                  // OLED display

String fileName="empty";                                      // filename that will be displayed on screen
bool screen = true;

Adafruit_BMP085 bmp;                                          // get ext temp
float ExtTempC;                                               // get ext temp
float ExtPressPa;                                             // get ext temp

#define USERNAME "fpuhl"                                      // Thinger
#define DEVICE_ID "Harpia"                                    // Thinger
#define DEVICE_CREDENTIAL "Ck5H_k7JNSyLoU-0"                  // Thinger
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);   // Thinger

Scheduler runner;                                             // Create the Scheduler that will be in charge of managing the tasks
Task task1(1*60*1000, TASK_FOREVER, &getWifiStatus);          // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task task2(2*60*1000, TASK_FOREVER, &DateandTimerequest);     // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task task3(1*60*1000, TASK_FOREVER, &updateBME180);           // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task task4(10*60*1000, TASK_FOREVER, &takeImage);             // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task task5(30*60*1000, TASK_FOREVER, &CardHealth);            // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task task6(1*1*3000, TASK_FOREVER, &screenLoop);              // Create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function


/*************************************************************************************
 * Setup
*************************************************************************************/

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 				          //disable brownout detector
 
  Serial.begin(115200);
    delay(1000);                                  			      // delay to stabilize the serial comm

  configTime(-10800, 0, "pool.ntp.org");          			      // NPT

  display.displayOn();                                        // OLED Display
  display.init();                                             // OLED Display

  Wire.begin (1, 3);                                          // BMP180
  bmp.begin();                                                // BMP180
  
  getSWversion();                                             // Read SW version 

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, ino);
    display.display();
      delay(1000);
  display.displayOff();                                       // Turn off display because it is affecting wifi coonection
  
  wifiConnect();                                  			      // Connect to wifi network

    display.displayOn();
    display.drawString(0, 13, "Wifi Connected");
    display.display();
      delay(1000);

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); // Firebase

  config.api_key = API_KEY;                       			      // Firebase: Assign the api key (required)
  auth.user.email = USER_EMAIL;                   			      // Firebase: Assign the user sign in credentials
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback; 		    // Firebase: Assign the callback function for the long running token generation task
															                                // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);                 			      // Firebase
  Firebase.reconnectWiFi(true);                   			      // Firebase                
  
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
    config.frame_size = FRAMESIZE_UXGA; 						          // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
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
      delay(1000);
    return;
  }else{
    display.displayOn();
    display.drawString(0, 26, "Camera initiated");
    display.display();
      delay(1000);
  }
  
  //Serial.println("Starting SD Card");
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
    return;
  }else{
    display.displayOn();
    display.drawString(0, 39, "Card Mounted");
    display.display();
      delay(1000);
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }

    /* First request before main loop just to feed variables */
    getWifiStatus();                                        // refresh wifi network status 
    DateandTimerequest();                                   // get date&time from Network
    takeImage();                                            // take image from camera
    CardHealth();                                           // Check SD card status
    updateBME180();                                         // Get External Temperature
    ThingerDefinitions();

  runner.addTask(task1);                                    // Add the task to the task scheduler
  runner.addTask(task2);
  runner.addTask(task3);
  runner.addTask(task4);
  runner.addTask(task5);
  runner.addTask(task6);
  
  task1.enable();                                           // Activate the task
  task2.enable(); 
  task3.enable(); 
  task4.enable(); 
  task5.enable();
  task6.enable();    
}

/*************************************************************************************
 * Main loop
*************************************************************************************/

void loop() {

  runner.execute();
  thing.handle();
  //unsigned long currentMillis = millis();
 
  if(error_counter>=100){               						        // if error counting exceeds threshold, then restart the system         
    ESP.restart();
  }

}


void screenLoop() {
    
    //previousMillis_screenLoop = currentMillis;
   
    display.init();                                         // Display init to clear it
    display.flipScreenVertically();  
      
      if(screen == true){
        showScreenA();
        screen = false;
      }else{
        showScreenB();
        screen = true;
      }
      
    display.display();

}  
