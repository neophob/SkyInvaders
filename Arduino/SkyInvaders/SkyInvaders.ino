/*
 * SkyInvaders firmware, Copyright (C) 2012 michael vogt <michu@neophob.com>
 *
 * This file is part of SkyInvaders. 687/422 
 *
 * ExpeditInvaders is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * ExpeditInvaders is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */


/*
Version 1.0 of the Arduino IDE introduced the F() syntax for storing strings in flash memory rather than RAM. e.g.
*/

// FEATURES

//WOL needs 1164 bytes 
#define USE_WOL 1

//use DHCP server OR static IP, dhcp needs 3148 bytes eeprom
//#define USE_DHCP 1

//use serial debug or not, needs 3836 bytes
#define USE_SERIAL_DEBUG 1

//enable basic osc server
#define USE_OSC 1

//enable the option to decrypt an encrypted OSC packet 46 bytes ram
#define USE_OSC_DECRYPTION 1



//default Pixels
#define NR_OF_PIXELS 160

// maximal sleep time for the color animation
#define MAX_SLEEP_TIME 250.0f

//SkyInvaders knows 3 Animation Mode (Static Color, Color Fade, Serverimage)
#define MAX_NR_OF_MODES 3

// define strip hardware, use only ONE hardware type
#define USE_WS2801 1
//#define USE_LPD8806 1

//TODO replace it with your OSC server
#define OSC_SERVER "192.168.111.21" 


#include <SPI.h>
#include <Ethernet.h>

#ifdef USE_OSC
//make sure you enabled strings (#define _USE_STRING_) in OSCcommon.h
#include <ArdOSC.h>
#include <Dns.h>
#endif

//used as we need to send out raw socket stuff
#ifdef USE_WOL
#include <EthernetUdp.h>
#include <utility/w5100.h>
#include <utility/socket.h>
#endif


#ifdef USE_WS2801
  #include <WS2801.h>
#endif
#ifdef USE_LPD8806
  #include <LPD8806.h>
#endif  

/**************
 * STRIP
 **************/
//output pixels data/clock3/2
#define dataPin 7
#define clockPin 6

//dummy init the pixel lib
#ifdef USE_WS2801
WS2801 strip = WS2801(); 
#endif
#ifdef USE_LPD8806
LPD8806 strip = LPD8806(); 
#endif  

/**************
 * NETWORK
 **************/
#ifndef USE_DHCP
byte myIp[]  = { 
  192, 168, 111, 177 };
#endif

byte myMac[] = { 
  0xde, 0xad, 0xbe, 0xef, 0xbe, 0x01 };

#ifdef USE_WOL
EthernetUDP Udp;
#endif


/**************
 * OSC
 **************/
#ifdef USE_OSC

const int serverPort  = 10000;
OSCServer oscServer;

//this array will be filled by the dns client
byte oscServerIp[]  = { 
  0,0,0,0 };

IPAddress serverIp;
OSCClient client; //TODO ADDME

#ifdef USE_OSC_DECRYPTION
#define ARDUINO_LISTENING_ENCRYPTION_PORT 7999
EthernetServer decryptionServer(ARDUINO_LISTENING_ENCRYPTION_PORT);
#endif

//change display mode, 3 modes: static color, color fade, serverimage
#define OSC_MSG_MODE "/mod" 

//change color of static color mode
#define OSC_MSG_STATIC_COL_R "/r" 
#define OSC_MSG_STATIC_COL_G "/g" 
#define OSC_MSG_STATIC_COL_B "/b" 

#define OSC_MSG_COLFADE_COLORSET "/cos" 

//generic animation speed
#define OSC_MSG_GENERIC_SPEED "/spd" 

#define OSC_MSG_WOL "/wol" 

#define OSC_MSG_UPDATE_PIXEL "/pxl" 
#endif 


/**************
 * BUSINESS LOGIC
 **************/
const uint8_t MODE_STATIC_COLOR = 0;
const uint8_t MODE_COLOR_FADE = 1;
const uint8_t MODE_SERVER_IMAGE = 2;

//animation mode
uint8_t oscMode=MODE_STATIC_COLOR;

//which color set is selected?
uint8_t oscColorSetNr=0;

//static colorvalues
uint8_t oscR=255, oscG=255, oscB=255;
uint32_t staticColor=0xffffff;

//update strip after DELAY (in ms)
uint8_t oscDelay = 20;

//current delay value
uint8_t currentDelay = 0;

uint16_t frame = 0;
/**************
 * MISC
 **************/
const uint8_t ledPin = 9;

/**************
 * SETUP
 **************/
void setup(){ 

#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);  
#endif

// --init strip-------------------------------------

#ifdef USE_WS2801
  #ifdef USE_SERIAL_DEBUG
    Serial.println(F("WS2801"));
  #endif
  strip = WS2801(NR_OF_PIXELS, dataPin, clockPin);   
#endif

#ifdef USE_LPD8806
  #ifdef USE_SERIAL_DEBUG
    Serial.println(F("INV8806"));
  #endif
  strip = LPD8806(NR_OF_PIXELS, dataPin, clockPin); 
#endif

  #ifdef USE_SERIAL_DEBUG
    Serial.print(F("# Pixel: "));
    Serial.println(NR_OF_PIXELS, DEC);
  #endif

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Free Mem: "));
  Serial.println(freeRam(), DEC);
#endif  

// --DHCP-------------------------------------
#ifdef USE_DHCP

  #ifdef USE_SERIAL_DEBUG
    Serial.println(F("Use DHCP"));
  #endif

  //start Ethernet library using dhcp
  if (Ethernet.begin(myMac) == 0) {
    #ifdef USE_SERIAL_DEBUG
      Serial.println(F("No DHCP Server found"));
    #endif
    // no point in carrying on, so do nothing forevermore:
    for(;;);
  }
#else
// --manual ip-------------------------------------

  #ifdef USE_SERIAL_DEBUG
    Serial.println(F("Use Manual IP"));
  #endif
  //Manual IP
  Ethernet.begin(myMac, myIp);
#endif 


#ifdef USE_SERIAL_DEBUG
  Serial.print(F("IP: "));
  for (byte i = 0; i < 4; i++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[i], DEC);
    Serial.print(F("."));
  }
  Serial.println();
#endif

// --dns-------------------------------------

#ifdef USE_OSC
  //get ip from dns name, used to find osc server
  DNSClient dns;
  
  dns.begin(Ethernet.dnsServerIP());
  int ret = dns.getHostByName(OSC_SERVER, serverIp);
  if (ret == 1) {
    
#ifdef USE_SERIAL_DEBUG
    Serial.print(F("OSC IP:"));
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      // print the value of each byte of the IP address:
      Serial.print(serverIp[thisByte], DEC);

      //get ip 
      oscServerIp[thisByte] = serverIp[thisByte];
      Serial.print(F(".")); 
    }
    Serial.println();
 #endif 
  //dns osc client done

    
  } else {
#ifdef USE_SERIAL_DEBUG    
    Serial.print(F("Failed to resolve hostname: "));
    Serial.println(ret, DEC);
#endif    
  }
#endif


// --led strip-------------------------------------
#ifdef USE_SERIAL_DEBUG    
    Serial.println(F("Start LED Strip"));
#endif
  //start strips 
  strip.begin();
  strip.show();


// --start udp server-------------------------------------

#ifdef USE_WOL
    #ifdef USE_SERIAL_DEBUG    
        Serial.println(F("Start UDP Server"));
    #endif
    
    //init UDP, used for WOL
    Udp.begin(7);
#endif

// --start osc server-------------------------------------
#ifdef USE_OSC

#ifdef USE_SERIAL_DEBUG    
    Serial.println(F("Start OSC Server"));
#endif

  //start osc server
  oscServer.begin(serverPort);
  //register OSC callback function
  oscServer.addCallback(OSC_MSG_STATIC_COL_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_STATIC_COL_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_STATIC_COL_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_MODE, &oscCallbackChangeMode); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_COLFADE_COLORSET, &oscCallbackColorSet); //PARAMETER: 1, float value 0..1  
  oscServer.addCallback(OSC_MSG_GENERIC_SPEED, &oscCallbackSpeed); //PARAMETER: 1, float value 0..1  
#ifdef USE_WOL  
  oscServer.addCallback(OSC_MSG_WOL, &oscCallbackWol); //PARAMETER: 1, float value 0..1  
#endif
  oscServer.addCallback(OSC_MSG_UPDATE_PIXEL, &oscCallbackPixel); //PARAMETER: 2, int offset, 4xlong

#ifdef USE_OSC_DECRYPTION
#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Init TCP Server on Port "));
  Serial.println(ARDUINO_LISTENING_ENCRYPTION_PORT, DEC);
#endif
  decryptionServer.begin();
  
#endif


#endif
// --start animation mode-------------------------------------

  //just to be sure!
  loadColorSet(0);
  //init animation mode
  initAnimationMode();

  //let the onboard arduino led blink
  pinMode(ledPin, OUTPUT);  
  synchronousBlink();

#ifdef USE_SERIAL_DEBUG
  Serial.print(F("Free Mem: "));
  Serial.println(freeRam(), DEC);
#endif  
}




/**************
 * LOOP
 **************/
void loop(){  
#ifdef USE_OSC
  if (oscServer.aviableCheck()>0){
    //we need to call available check to update the osc server
  }
#endif

  //check if the effect should be updated or not
  if (currentDelay>0) {
    //delay not finished yet - do not modify the strip but read network messages
    currentDelay--;
    delay(1);
    
#ifdef USE_OSC
#ifdef USE_OSC_DECRYPTION
    //get encrypted tcp traffic 
    handleEncryptedTraffic();
#endif
#endif
  } 
  else {
    //delay finished, update the strip content
    currentDelay=oscDelay;    
    loopAnimationMode();
    strip.show();
    frame++;
    
#ifdef USE_OSC    
    if (frame%5==1) {
//    if (frame%50000==1) {
#ifdef USE_SERIAL_DEBUG
  Serial.println(F("OSC Ping"));
#endif  
      sendOscPingToServer();
      
  Serial.print(F("Free Mem: "));
  Serial.print(freeRam(), DEC);
  Serial.print(" ");
  Serial.println(millis(), DEC);
      
    }
#endif

  }
  
}








