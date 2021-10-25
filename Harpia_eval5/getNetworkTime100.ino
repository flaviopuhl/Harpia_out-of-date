void DateandTimerequest(){

  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  } else {
  
  char monthDay[3];
  char currentMonth[3];
  char currentYear[3];
  char currentHour[3];
  char currentMin[3];
  char currentSec[3];

    strftime(monthDay,3, "%d", &timeinfo);
    strftime(currentMonth,3, "%m", &timeinfo);
    strftime(currentYear,3, "%Y", &timeinfo);
    strftime(currentHour,3, "%H", &timeinfo);
    strftime(currentMin,3, "%M", &timeinfo);
    strftime(currentSec,3, "%S", &timeinfo);

 Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  currentTime = String(currentHour)+                         // Must be declared as global
                       "-" +
                       String(currentMin)+
                       "-" +
                       String(currentSec); 

  currentDate = String (monthDay)+                           // Must be declared as global
                       "-" +
                       String(currentMonth)+
                       "-" +
                       String(currentYear); 

  currentFullDate = currentDate +" "+currentTime;           // Must be declared as global                    
  
  Serial.println("");
  Serial.println("############### Current Time ###############\n");
  Serial.print("Epoch Time        : ");  Serial.println("-");
  Serial.print("Current Date      : ");  Serial.println(currentDate);
  Serial.print("Current Time      : ");  Serial.println(currentTime);
  Serial.print("Current Full Date : ");  Serial.println(currentFullDate);
  Serial.println("");

  }
 
}
