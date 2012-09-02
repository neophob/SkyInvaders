import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;
Random r;
// roove of concept, send pixelinformation as osc message (OFFSET, PIXEL1, PIXEL2, PIXEL3, PIXEL4)
// take less than 5ms to send 320 32bit pixe data.


void setup() {
  size(400,400);
  frameRate(1);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this,12000);
  
  //CHANGE to your ip
  myRemoteLocation = new NetAddress("192.168.111.23",10000);
  
  r = new Random();
}


void draw() {

  long now=System.currentTimeMillis();  
  for (int i=0; i<96; i+=4) {
    long l1=r.nextLong()%0xffffff;
/*    long l2=r.nextLong()%0xffffff;
    long l3=r.nextLong()%0xffffff;
    long l4=r.nextLong()%0xffffff;
    Object[] oscParam = new Object[] {0, l1, l2, l3, l4};
    oscP5.send("/pxl", oscParam, myRemoteLocation);*/
    oscP5.send("/pxl",new Object[] {i, 0xffffff, 0x00aabb, 0x997755, 0xaffe00}, myRemoteLocation);
  }
  long needed = System.currentTimeMillis()-now;
  println("Sendtime: "+needed+"ms");
}

