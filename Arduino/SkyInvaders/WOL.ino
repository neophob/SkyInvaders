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
    //NOTE: BUG here, we cannot send more than 46 bytes...
    for (i=0; i<16; i++) {
      Udp.write(mac, sizeof mac);
    }
    
    //Called after writing UDP data to the remote connection.
    Udp.endPacket();
#ifdef USE_SERIAL_DEBUG
  Serial.println("done!");
#endif 
    
}


/*
void SendWOLMagicPacket(byte * pMacAddress)
{
  // The magic packet data sent to wake the remote machine. Target machine's
  // MAC address will be composited in here.
  const int nMagicPacketLength = 102;
  byte abyMagicPacket[nMagicPacketLength] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  byte abyTargetIPAddress[] = { 255, 255, 255, 255 }; // don't seem to need a real ip address.
  const int nWOLPort = 7;
  const int nLocalPort = 8888; // to "listen" on (only needed to initialize udp)
 
 
  // Compose magic packet to wake remote machine. 
  for (int ix=6; ix<102; ix++)
    abyMagicPacket[ix]=pMacAddress[ix%6];
 
  if (UDP_RawSendto(abyMagicPacket, nMagicPacketLength, nLocalPort, 
  abyTargetIPAddress, nWOLPort) != nMagicPacketLength)
    Serial.println("Error sending WOL packet");
}
int UDP_RawSendto(byte* pDataPacket, int nPacketLength, int nLocalPort, byte* pRemoteIP, int nRemotePort)
{
  int nResult;
  int nSocketId; // Socket ID for Wiz5100
 
  // Find a free socket id.
  nSocketId = MAX_SOCK_NUM;
  for (int i = 0; i < MAX_SOCK_NUM; i++) 
  {
    uint8_t s = w5100.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) 
	  {
      nSocketId = i;
      break;
    }
  }
 
  if (nSocketId == MAX_SOCK_NUM)
    return 0; // couldn't find one. 
 
  if (socket(nSocketId, SnMR::UDP, nLocalPort, 0))
  {
    nResult = sendto(nSocketId,(unsigned char*)pDataPacket,nPacketLength,(unsigned char*)pRemoteIP,nRemotePort);
    close(nSocketId);
  } else
    nResult = 0;
 
  return nResult;
}*/

