
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

#ifdef USE_SERIAL_DEBUG 

int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

/*
int freeRam() {
  int byteCounter = 0; // initialize a counter
  byte *byteArray; // create a pointer to a byte array
  // More on pointers here: http://en.wikipedia.org/wiki/Pointer#C_pointers

  // use the malloc function to repeatedly attempt allocating a certain number of bytes to memory
  // More on malloc here: http://en.wikipedia.org/wiki/Malloc
  while ( (byteArray = (byte*) malloc (byteCounter * sizeof(byte))) != NULL ) {
    byteCounter++; // if allocation was successful, then up the count for the next try
    free(byteArray); // free memory after allocating it
  }
  
  free(byteArray); // also free memory after the function finishes
  
  return byteCounter; // send back the highest number of bytes successfully allocated
}*/
#endif


