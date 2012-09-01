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
  int16_t argCount = _mes->getArgsNum();
  if (argCount<5) {
#ifdef USE_SERIAL_DEBUG
    Serial.print("Invalid Parametercount: ");
    Serial.println(argCount, DEC);
#endif     
  }
  int32_t ofs = _mes->getArgInt32(0);
#ifdef USE_SERIAL_DEBUG
    Serial.print("OSC Update Pixel, offset: ");
    Serial.println(ofs, DEC);
#endif     
  
  //get 4 colors from osc message
  for (byte b=0; b<4; b++) {
    strip.setPixelColor(ofs+b, _mes->getArgInt32(b));
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
  //send magic packet
  WOL(mac);
}


void oscCallbackPixel(OSCMessage *_mes){
  oscB = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print("B:");
  Serial.println(oscB, DEC);
#endif  
}

