/*
 * Source found here: https://github.com/mikispag/arduino-WakeOnLan/blob/master/arduino-WakeOnLan.ino
 * See http://en.wikipedia.org/wiki/Wake-on-LAN for more information
 */

byte broadcastAddress[] = { 255, 255, 255, 255 };    //to verify!

//send wol magic pajet
void WOL(byte mac[]) {
#ifdef USE_SERIAL_DEBUG
  Serial.print("Send WOL... ");
#endif 
  
    byte i;
    //Starts a connection to write UDP data to the remote connection, parameter: remoteIP, remotePort
    //Hint about port 7: Since the magic packet is only scanned for the string above, and not actually 
    //parsed by a full protocol stack, it may be sent as any network- and transport-layer protocol, although 
    //it is typically sent as a UDP datagram to port 7 or 9
    Udp.beginPacket(broadcastAddress, 7);
    
    //Writes UDP data to the remote connection. Must be wrapped between beginPacket() and endPacket().

    //Start of WOL magic paket (preamble)
    for (i=0; i<6; i++) {
      Udp.write(0xff);
    }
    
    //the wol packet contains 16 times the mac address  
    for (i=0; i<16; i++) {
      Udp.write(mac, sizeof mac);
    }
    
    //Called after writing UDP data to the remote connection.
    Udp.endPacket();
#ifdef USE_SERIAL_DEBUG
  Serial.println("done!");
#endif 
    
}

