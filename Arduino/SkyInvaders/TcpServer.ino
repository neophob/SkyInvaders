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


void handleEncryptedTraffic() {
  EthernetClient client = server.available();
  if (client) {
    
    while (client.connected()) {
      if (client.available()) {
        //maybe read header from package
      }  
    }
  }
  
}
