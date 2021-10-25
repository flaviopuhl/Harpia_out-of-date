void CardHealth()
{
  
    cardTotalBytes = SD_MMC.totalBytes() / (1024 * 1024);     // Declare as global
    cardUsedBytes = SD_MMC.usedBytes() / (1024 * 1024);       // Declare as global
    cardUsedSpace = (cardUsedBytes*100)/cardTotalBytes;       // Declare as global
  
      //Serial.println("");
      //Serial.println("############### Card Health ################");
      //Serial.print("Total space (Mb) : "); Serial.println(cardTotalBytes);
      //Serial.print("Used space (Mb)  : "); Serial.println(cardUsedBytes);
      //Serial.print("Used space (%)   : "); Serial.println(cardUsedSpace);
          
}
