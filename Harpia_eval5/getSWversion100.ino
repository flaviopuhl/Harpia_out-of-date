void getSWversion(){
                                                  
  ino = (ino.substring((ino.indexOf(".")), (ino.lastIndexOf("\\")) + 1));   // Must be declared as global
  
  Serial.println("");
  Serial.println("################ SW Version ################\n");
  Serial.print("Software Version  : "); Serial.println(ino);
}
