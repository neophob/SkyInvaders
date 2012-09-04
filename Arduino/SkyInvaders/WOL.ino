/*
 * Source found here: https://github.com/mikispag/arduino-WakeOnLan/blob/master/arduino-WakeOnLan.ino
 * See http://en.wikipedia.org/wiki/Wake-on-LAN for more information
 */

//Src: http://www.blueleafsoftware.com/Resources/EmbeddedSand/Wake_on_LAN
//I made the code compatible with Arduino v1.0.1
void SendWOLMagicPacket(byte * pMacAddress) {
  
#ifdef USE_SERIAL_DEBUG
  Serial.print("Send WOL... ");
#endif 

  // The magic packet data sent to wake the remote machine. Target machine's
  // MAC address will be composited in here.
  const int nMagicPacketLength = 102;
  byte abyMagicPacket[nMagicPacketLength] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  byte abyTargetIPAddress[] = { 255, 255, 255, 255 }; // don't seem to need a real ip address.
  const int nWOLPort = 9;
  const int nLocalPort = 8888; // to "listen" on (only needed to initialize udp)
 
  // Compose magic packet to wake remote machine. 
  for (int ix=6; ix<102; ix++) {
    abyMagicPacket[ix]=pMacAddress[ix%6];
  }
  
  if (UDP_RawSendto(abyMagicPacket, nMagicPacketLength, nLocalPort, abyTargetIPAddress, nWOLPort) != nMagicPacketLength) {
#ifdef USE_SERIAL_DEBUG
    Serial.println("Error sending WOL packet");
#endif
  }
}


int UDP_RawSendto(byte* pDataPacket, int nPacketLength, int nLocalPort, byte* pRemoteIP, int nRemotePort) {
  int nResult;
  int nSocketId; // Socket ID for Wiz5100
 
  // Find a free socket id.
  nSocketId = MAX_SOCK_NUM;
  for (byte i = 0; i < MAX_SOCK_NUM; i++) {
    uint8_t s = W5100.readSnSR(i);
    if (s == SnSR::CLOSED || s == SnSR::FIN_WAIT) {
      nSocketId = i;
      break;
    }
  }
 
  if (nSocketId == MAX_SOCK_NUM) {
    return 0; // couldn't find one. 
  }
  
  if (socket(nSocketId, SnMR::UDP, nLocalPort, 0)) {
    nResult = sendto(nSocketId,(unsigned char*)pDataPacket,nPacketLength,(unsigned char*)pRemoteIP,nRemotePort);
    close(nSocketId);
  } else {
    nResult = 0;
  }
  
  return nResult;
}


