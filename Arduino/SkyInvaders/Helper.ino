
//blink the arduino onboard led
void synchronousBlink() {
  digitalWrite(ledPin, HIGH);
  delay(20);
  digitalWrite(ledPin, LOW);  
}


// Create a 24 bit color value from R,G,B
uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  uint32_t c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//update static color
void updateStaticColor() {
  staticColor = Color(oscR,oscG,oscB);
}


//convert char to byte 
//Src: http://stackoverflow.com/questions/1557400/hex-to-char-array-in-c
byte xdigit( char digit ){
  int val;
       if( '0' <= digit && digit <= '9' ) val = digit -'0';
  else if( 'a' <= digit && digit <= 'f' ) val = digit -'a'+10;
  else if( 'A' <= digit && digit <= 'F' ) val = digit -'A'+10;
  else                                    val = -1;
  return val;
}


int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


// --------------------------- Debug
void logDebugPrintln(const char s[]) {
  logDebugPrint(s);
#ifdef USE_SERIAL_DEBUG 
  Serial.println();
#endif
}


void logDebugPrint(const char s[]) {
#ifdef USE_SERIAL_DEBUG 
  Serial.print(s);
#endif

  Udp.beginPacket(broadcastAddress, REMOTE_PORT); 
  Udp.write(s);
  Udp.endPacket();
}


void logDebugPrintln(unsigned long n, uint8_t base) {
  logDebugPrint(n, base);
#ifdef USE_SERIAL_DEBUG 
  Serial.println();
#endif
}


void logDebugPrint(unsigned long n, uint8_t base) {
#ifdef USE_SERIAL_DEBUG 
  Serial.print(n, base);
#endif

  Udp.beginPacket(broadcastAddress, REMOTE_PORT); 
  Udp.write(n);
  Udp.endPacket();

}


