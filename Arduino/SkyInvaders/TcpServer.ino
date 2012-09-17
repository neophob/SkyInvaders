//TCP Server, handle encrypted traffic

/*

Example RAW OSC Paket Dumps


WOL Packet (36 Bytes)
---------------------
sendtyped /wol s 705681b1dbb3
0000   2f 77 6f 6c 00 00 00 00 2c 73 00 00 37 30 35 36  /wol....,s..7056
0010   38 31 62 31 64 62 62 33 00 00 00 00              81b1dbb3....
as Hex String: 2f776f6c000000002c73000037303536383162316462623300000000


Set 4 Pixels (28 Bytes)
-----------------------
sendtyped /pxl iiiii 22 16711935 16711935 16711935 16711935
0000   2f 70 78 6c 00 00 00 00 2c 69 69 69 69 69 00 00  /pxl....,iiiii..
0010   00 00 00 16 00 ff 00 ff 00 ff 00 ff 00 ff 00 ff  ................
0020   00 ff 00 ff                                      ....


*/
#ifdef USE_OSC

#ifdef USE_OSC_DECRYPTION

#define SIZE_ENCRYPTED_PACKAGE 80

void handleEncryptedTraffic() {
  EthernetClient client = decryptionServer.available();
  if (client) {
    
    while (client.connected()) {
      if (client.available()) {
        //maybe read header from package
                
        uint8_t encryptedOscPacket[SIZE_ENCRYPTED_PACKAGE];
        int readBytesFromLan = client.read(encryptedOscPacket, SIZE_ENCRYPTED_PACKAGE);
        if (readBytesFromLan < 0) {
           //nothing recieved
           return; 
        }
        
        //TODO make method out of this
        //-- Start decrypt
        for (uint8_t n=0; n<readBytesFromLan; n++) {
           encryptedOscPacket[n] = encryptedOscPacket[n]^12;
        }
        //-- End decrypt
        
        if (oscServer.processRawData(encryptedOscPacket) < 0) {
          #ifdef USE_SERIAL_DEBUG
            Serial.println(F("Failed to parse OSC Message"));
          #endif          
        } else {
          #ifdef USE_SERIAL_DEBUG
            Serial.println(F("Handled OSC Message"));
          #endif          
        }
      }  
    }
  }
  
}

#endif
#endif

