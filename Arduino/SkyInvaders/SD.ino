#ifdef USE_SD
//use this lib: http://henningkarlsen.com/electronics/library.php?id=37

// set up variables using the SD utility library functions:

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = 4;


void initSdCardInformation() {
Sd2Card card;
SdVolume volume;
SdFile root;
  
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);     // change this to 53 on a mega

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
#ifdef USE_SERIAL_DEBUG    
    Serial.println(F("initialization failed. Things to check:"));
//    Serial.println("* is a card is inserted?");
//    Serial.println("* Is your wiring correct?");
//    Serial.println("* did you change the chipSelect pin to match your shield or module?");
#endif
    return;
  } 

#ifdef USE_SERIAL_DEBUG    
  // print the type of card
  Serial.print(F("Init done, Card type: "));
  Serial.println(card.type(), HEX);
#endif  

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
#ifdef USE_SERIAL_DEBUG 
    Serial.println(F("Could not find FAT16/FAT32 partition."));
#endif  
    return;
  }

#ifdef USE_SERIAL_DEBUG
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print(F("\nVolume type is FAT"));
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
//  Serial.print("Volume size (bytes): ");
//  Serial.println(volumesize);
  Serial.print(F("Volume size (Kbytes): "));  
  volumesize /= 1024;
  Serial.println(volumesize);

  Serial.println(F("\nFiles found on the card (name, date and size in bytes): "));
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
#endif

}


void readFileFromSd() {

  if (!sdFileRead) {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    sdFileRead = SD.open("datalog.txt");    
    
    if (!sdFileRead) {
#ifdef USE_SERIAL_DEBUG      
      Serial.println(F("error opening datalog.txt"));
#endif      
      return;
    }
  }

  for (unsigned int i=0; i < strip.numPixels(); i++) {
      if (!sdFileRead.available()) {
        sdFileRead.seek(0);
      }
      //read line
  }
    
}
#endif
