
uint32_t colorsetColors[3];

//get array size
#define nrOfColorsInArray (sizeof(colorsetColors)/sizeof(unsigned long)) 

#define MAX_COLOR_MODE 6

 byte boarderCount;
 byte arrayCount;


//----------------------------
//init color set
//----------------------------
void initColorSet(unsigned long col[3]) {
#ifdef USE_SERIAL_DEBUG
  Serial.println(F("initColorSet"));
#endif 
  
  //copy array.. memcpy? never heard of it...
  for (byte b=0; b<3; b++) {
    colorsetColors[b] = col[b];
  }

  arrayCount = nrOfColorsInArray;
  boarderCount = 255 / arrayCount;

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("boarderCount: "));
  Serial.println(boarderCount);
#endif 
}

//----------------------------
//calc smooth color
//----------------------------
unsigned int getSmoothColor(byte pos) {
  byte ofs=0;
  while (pos>boarderCount) {
    pos -= boarderCount;
    ofs++;
  }

  byte targetOfs = ofs+1;
  return calcSmoothColor(colorsetColors[targetOfs%arrayCount], colorsetColors[ofs%arrayCount], pos);
}

//----------------------------
//calc smooth color
//----------------------------
unsigned int calcSmoothColor(unsigned long col1, unsigned long col2, byte pos) {
  unsigned int b= col1&255;
  unsigned int g=(col1>>8)&255;
  unsigned int r=(col1>>16)&255;
  unsigned int b2= col2&255;
  unsigned int g2=(col2>>8)&255;
  unsigned int r2=(col2>>16)&255;

  unsigned int mulCol=pos*arrayCount;        //extend pos to 255, so pos is between 0..255
  unsigned int oppositeColor = 255-mulCol;

  //I got a fancy error message for Error: even register number required 
  r=((r*mulCol)>>8) + ((r2*oppositeColor)>>8);
  g=((g*mulCol)>>8) + ((g2*oppositeColor)>>8);
  b=((b*mulCol)>>8) + ((b2*oppositeColor)>>8);

  //verify color order
  return Color(b,r,g);
}


//----------------------------
//load (static) colorset
//----------------------------
void loadColorSet(byte colorMode) {
  unsigned long initialColor[3] = { 
    0xff0000, 0x00ff00, 0x0000ff   }; //RGB
 
  switch (colorMode) {
  case 1:
    initialColor[0] = 0xdc323c;  //Rasta
    initialColor[1] = 0xf0cb58;
    initialColor[2] = 0x3c825e;
    break; 
  case 2:
    initialColor[0] = 0xd3517d;  //CGA
    initialColor[1] = 0x15a0bf;
    initialColor[2] = 0xffc062;
    break; 
  case 3:
    initialColor[0] = 0x008c53;  //Brazil
    initialColor[1] = 0x2e00e4;
    initialColor[2] = 0xdfea00;  
    break; 
  case 4:
    initialColor[0] = 0x588F27;  //Fizz
    initialColor[1] = 0x04BFBF;
    initialColor[2] = 0xF7E967;  
    break; 
  case 5:
    initialColor[0] = 0x9f456b;  //Kitty
    initialColor[1] = 0x4f7a9a;
    initialColor[2] = 0xe6c84c;  
    break; 
/*  case 6:
    initialColor[0] = 0x323228;  //Neon
    initialColor[1] = 0x717155;
    initialColor[2] = 0xb4dc00;  
    break; 
  case 7:
    initialColor[0] = 0x000000;  //Lantern
    initialColor[1] = 0x0d9a0d;
    initialColor[2] = 0xffffff;  
    break; 
  case 8:
    initialColor[0] = 0x0000ff;  //Lemming
    initialColor[1] = 0x00ff00;
    initialColor[2] = 0xffffff;  
    break; 
  case 9:
    initialColor[0] = 0x3e3e3e;  //LeBron
    initialColor[1] = 0xd4b600;
    initialColor[2] = 0xffffff;  
    break; */
  }
#ifdef USE_SERIAL_DEBUG      
  Serial.print(F("load new colorset: "));
  Serial.println(colorMode);      
#endif
  initColorSet(initialColor);
}

