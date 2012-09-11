

//WTF! the ifdec directive does not work here!

#ifdef USE_OSC    

//convert the first osc argument and return byte
byte getFirstFloatArgument(OSCMessage *_mes) {
  return byte( _mes->getArgFloat(0)*255.f );
}


void oscCallbackSpeed(OSCMessage *_mes){
  oscDelay = byte( _mes->getArgFloat(0)*MAX_SLEEP_TIME );

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Speed: "));
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
  Serial.print(F("Mode: "));
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
  Serial.print(F("ColorSet: "));
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
  Serial.print(F("R: "));
  Serial.println(oscR, DEC);
#endif

}


// G
void oscCallbackG(OSCMessage *_mes){
  oscG = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print("G: ");
  Serial.println(oscG, DEC);
#endif
}


// B
void oscCallbackB(OSCMessage *_mes){
  oscB = getFirstFloatArgument(_mes);
  updateStaticColor();

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("B: "));
  Serial.println(oscB, DEC);
#endif

}

#ifdef USE_WOL
// WOL
void oscCallbackWol(OSCMessage *_mes){
  //get length of the parameter
  int16_t strSize=_mes->getArgStringSize(0);
  //verify messagelength (12 chars / 6 bytes)
  if (strSize!=13) {

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("WOL: Invalid parameter size "));
  Serial.println(strSize, DEC);
#endif

    return;
  }

  //get mac
  char tmpStr[12]; //string memory allocation
  _mes->getArgString(0, tmpStr);

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("WOL MAC"));
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
//  WOL(mac);
  SendWOLMagicPacket(mac);
}
#endif //USE_WOL


//update pixel via osc
void oscCallbackPixel(OSCMessage *_mes) {
  int16_t argCount = _mes->getArgsNum();
  if (argCount<5) {

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Invalid Parametercount: "));
  Serial.println(argCount, DEC);
#endif

    return;
  }
  
  //reset counter, used to check if we should switch to sd playback
  frame=0;
  
  //change mode to serverimage - else buffer would be overwritten!
  oscMode = MODE_SERVER_IMAGE;
  
  int32_t ofs = _mes->getArgInt32(0);

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("OSC Update Pixel, offset: "));
  Serial.println(ofs, DEC);
#endif
  
  //get 4 colors from osc message
  for (byte b=0; b<4; b++) {
    int32_t col=_mes->getArgInt32(b+1);
    strip.setPixelColor(ofs+b, col);

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Set: "));
  Serial.println(col, DEC);
#endif

  } 
}

//send a ping osc message to the server with the name OSC_SERVER
void sendOscPingToServer() {
  //loacal_mes,str is release by out of scope
  OSCMessage localMsg;
  
  localMsg.setAddress(oscServerIp, serverPort);
  localMsg.beginMessage("/ping");

  //maybe add own ip as parameter?
  
  //TODO ADD ME
  //client.send(&localMsg);  
}

#endif


/**/
