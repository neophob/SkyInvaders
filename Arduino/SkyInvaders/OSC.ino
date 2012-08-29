/**************** 
 * OSC Callbacks
 *
 */

//static colorvalues
uint8_t oscR=255, oscG=255, oscB=255;

//animation mode
uint8_t oscMode;

//update strip after DELAY
uint8_t oscDelay = 20;


//convert the first osc argument and return byte
byte getFirstFloatArgument(OSCMessage *_mes) {
  return byte( _mes->getArgFloat(0)*255.f );
}


void oscCallbackSpeed(OSCMessage *_mes){
  oscDelay = byte( _mes->getArgFloat(0)*MAX_SLEEP_TIME );

#ifdef USE_SERIAL_DEBUG
  Serial.print("D:");
  Serial.println(oscDelay, DEC);
#endif 
}


// change mode, use mode nr X
void oscCallbackChangeMode(OSCMessage *_mes){
  byte arg=byte(_mes->getArgFloat(0));  
  if (arg > MAX_NR_OF_MODES-1) {
    return;
  }

  oscMode=arg;
//  modeSave = mode;
//  initMode(); 
}


// R
void oscCallbackR(OSCMessage *_mes){
  oscR = getFirstFloatArgument(_mes);

#ifdef USE_SERIAL_DEBUG
  Serial.print("R:");
  Serial.println(oscR, DEC);
#endif 
}


// G
void oscCallbackG(OSCMessage *_mes){
  oscG = getFirstFloatArgument(_mes);

#ifdef USE_SERIAL_DEBUG
  Serial.print("G:");
  Serial.println(oscG, DEC);
#endif
}


// B
void oscCallbackB(OSCMessage *_mes){
  oscB = getFirstFloatArgument(_mes);

#ifdef USE_SERIAL_DEBUG
  Serial.print("B:");
  Serial.println(oscB, DEC);
#endif  
}