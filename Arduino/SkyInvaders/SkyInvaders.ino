/*
 * SkyInvaders firmware, Copyright (C) 2012 michael vogt <michu@neophob.com>
 *
 * This file is part of SkyInvaders.
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

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EEPROM.h>
#include <ArdOSC.h>
#include <WS2801.h>

//use serial debug or not
#define USE_SERIAL_DEBUG 1

//use DHCP server OR static IP
#define USE_DHCP 1

//default Pixels
#define NR_OF_PIXELS 160

// maximal sleep time for the color animation
#define MAX_SLEEP_TIME 160.0f

//SkyInvaders knows 3 Animation Mode (Static Color, Color Fade, Serverimage)
#define MAX_NR_OF_MODES 16

/**************
 * STRIP
 **************/
//output pixels data/clock3/2
int dataPin = 3; 
int clockPin = 2;  

//dummy init the pixel lib
WS2801 strip = WS2801(); 

/**************
 * NETWORK
 **************/
#ifndef USE_DHCP
byte myIp[]  = { 
  192, 168, 1, 111 };
#endif

byte myMac[] = { 
  0xde, 0xad, 0xbe, 0xef, 0xbe, 0x01 };

EthernetUDP Udp;

/**************
 * OSC
 **************/
const int serverPort  = 10000;
OSCServer oscServer;

//change display mode, 3 modes: static color, color fade, serverimage
#define OSC_MSG_MODE "/mode" 

//change color of static color mode
#define OSC_MSG_STATIC_COL_R "/colr" 
#define OSC_MSG_STATIC_COL_G "/colg" 
#define OSC_MSG_STATIC_COL_B "/colb" 

//animation speed for color fade
#define OSC_MSG_COLFADE_SPEED "/speed" 

/**************
 * BUSINESS LOGIC
 **************/
#define MODE_STATIC_COLOR 0
#define MODE_COLOR_FADE 1
#define MODE_SERVER_IMAGE 2

//animation mode
uint8_t oscMode=MODE_STATIC_COLOR;

//static colorvalues
uint8_t oscR=255, oscG=255, oscB=255;

//update strip after DELAY (in ms)
uint8_t oscDelay = 20;

//current delay value
uint8_t currentDelay = 0;

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
  Serial.println("Hello!");
#endif

  int cnt = NR_OF_PIXELS;

  //TODO check EEPROM

  strip = WS2801(cnt, dataPin, clockPin); 

  //ws2801 start strips 
  strip.begin();

  //DHCP, hint: we cannot use DHCP and manual IP together, out of space!
#ifdef USE_DHCP
  //start Ethernet library using dhcp
  if (Ethernet.begin(myMac) == 0) {
#ifdef USE_SERIAL_DEBUG
    Serial.println("Failed to configure Ethernet using DHCP");
#endif
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
#else
  //Manual IP
  Ethernet.begin(myMac, myIp)
#endif 

    //init UDP
    Udp.begin(7);

#ifdef USE_SERIAL_DEBUG 
  Serial.print("IP:");////32818
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
#endif

  //start osc server
  oscServer.begin(serverPort);
  //register OSC callback function
  oscServer.addCallback(OSC_MSG_STATIC_COL_R, &oscCallbackR); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_STATIC_COL_G, &oscCallbackG); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_STATIC_COL_B, &oscCallbackB); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_MODE, &oscCallbackChangeMode); //PARAMETER: 1, float value 0..1
  oscServer.addCallback(OSC_MSG_COLFADE_SPEED, &oscCallbackSpeed); //PARAMETER: 1, float value 0..1  

  //init animation mode
  initAnimationMode();

  //let the onboard arduino led blink
  pinMode(ledPin, OUTPUT);  
  synchronousBlink();
  delay(50);
  synchronousBlink();
}




/**************
 * LOOP
 **************/
void loop(){  

  if (oscServer.aviableCheck()>0){
    //we need to call available check to update the osc server
  }

  //check if the effect should be updated or not
  if (currentDelay>0) {
    //delay not finished yet - do not modify the strip but read network messages
    currentDelay--;
    delay(1);
  } 
  else {
    //delay finished, update the strip content
    currentDelay=oscDelay;
    
    loopAnimationMode();
  }
}






