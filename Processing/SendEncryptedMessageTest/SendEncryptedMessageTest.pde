import processing.net.*;

//Dump of a WOL OSC Packet
static String PACKET_DUMP_WOL = "2f776f6c000000002c73000037303536383162316462623300000000";
static String PACKET_DUMP_COLOR_R = "2f7200002c6600003f77efe0";
Client c;
byte[] rawData;

long startTs;

void setup() {
  println("Hello");
  
  //two packets/s
  frameRate(25);
  
//  c = new Client(this, "192.168.111.1", 7999); // Replace with your server's IP and port
  c = new Client(this, "192.168.111.22", 7999); // Replace with your server's IP and port
  //byte[] rawData = HexStringToByteArray(PACKET_DUMP_WOL);
  rawData = HexStringToByteArray(PACKET_DUMP_COLOR_R);
  
  //now "encrypt"
  for (int i=0; i<rawData.length; i++) {
     rawData[i] = byte(rawData[i]^12); 
  }
  
  println("Setup finished!");
  startTs = System.currentTimeMillis();
}

int frame=0;

//loop
void draw() {
  //print("send packet... ");
  c.write(rawData);  
  //println("done!");
  
  if (frame % 50 == 0) {
    long duration = System.currentTimeMillis()-startTs;
    duration /= 1000; //convert duration from ms to ms.
    float f = frame / (float)duration;
    println("Framerate :"+f);
  }
  
  frame++;
}


static byte[] HexStringToByteArray(String s) {
    byte data[] = new byte[s.length()/2];
    for(int i=0;i < s.length();i+=2) {
        data[i/2] = (Integer.decode("0x"+s.charAt(i)+s.charAt(i+1))).byteValue();
    }
    return data;
}

