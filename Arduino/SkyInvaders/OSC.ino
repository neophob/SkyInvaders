/**************** 
 * OSC Callbacks
 *
 */


//convert the first osc argument and return byte
byte getFirstFloatArgument(OSCMessage *_mes) {
  return byte( _mes->getArgFloat(0)*255.f );
}


void oscCallbackSpeed(OSCMessage *_mes){
  oscDelay = byte( _mes->getArgFloat(0)*MAX_SLEEP_TIME );

#ifdef USE_SERIAL_DEBUG
  Serial.print("Speed:");
  Serial.println(oscDelay, DEC);
#endif 
}


// change mode, use mode nr X
void oscCallbackChangeMode(OSCMessage *_mes){
  byte arg=byte(_mes->getArgFloat(0));  
  if (arg > MAX_NR_OF_MODES-1) {
    return;
  }

#ifdef USE_SERIAL_DEBUG
  Serial.print("Mode:");
  Serial.println(oscMode, DEC);
#endif 

  if (arg!=oscMode) {
    oscMode=arg;
    initAnimationMode();
  }  
}


void oscCallbackColorSet(OSCMessage *_mes){
  byte arg=byte(_mes->getArgFloat(0));  
  if (arg > MAX_COLOR_MODE-1) {
    return;
  }
#ifdef USE_SERIAL_DEBUG
  Serial.print("ColorSet:");
  Serial.println(oscColorSetNr, DEC);
#endif 

  //only if value change, load new colorset
  if (arg!=oscColorSetNr) {
    oscColorSetNr = arg;
    //load colorset
    loadColorSet(oscColorSetNr);    
  }

}



// R
void oscCallbackR(OSCMessage *_mes){
  oscR = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print("R:");
  Serial.println(oscR, DEC);
#endif 
}


// G
void oscCallbackG(OSCMessage *_mes){
  oscG = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print("G:");
  Serial.println(oscG, DEC);
#endif
}


// B
void oscCallbackB(OSCMessage *_mes){
  oscB = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print("B:");
  Serial.println(oscB, DEC);
#endif  
}

// WOL
void oscCallbackWol(OSCMessage *_mes){
  //get length of the parameter
  int16_t strSize=_mes->getArgStringSize(0);
  //verify messagelength (12 chars / 6 bytes)
  if (strSize!=13) {
#ifdef USE_SERIAL_DEBUG
    Serial.print("WOL: Invalid parameter size ");
    Serial.println(strSize, DEC);
#endif  
    return;
  }

  //get mac
  char tmpStr[12]; //string memory allocation
  _mes->getArgString(0, tmpStr);

#ifdef USE_SERIAL_DEBUG
  Serial.print("WOL MAC: ");
  Serial.println(tmpStr);
#endif  

  //now convert the 12byte chars ('d','e','a','d',...) to 6byte byte (0xde,0ad,...) array
  byte mac[6];
  byte ofs=0;
  for(byte i = 0; i < 12; i+=2) {
    byte sendb = xdigit(tmpStr[i+1]) + (xdigit(tmpStr[i]) << 4);
    mac[ofs++] = sendb;
  }
#ifdef USE_SERIAL_DEBUG
  Serial.println(mac[0], HEX);
  Serial.println(mac[1], HEX);
  Serial.println(mac[2], HEX);
  Serial.println(mac[3], HEX);
  Serial.println(mac[4], HEX);
  Serial.println(mac[5], HEX);
#endif  
  WOL(mac);

}


