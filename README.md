Hint:

# Important Note
Arduino v1.0.1 (and maybe newer) have a malloc/free bug!

Check:
http://code.google.com/p/arduino/issues/detail?id=468
http://code.google.com/p/arduino/issues/detail?id=857

## Howto fix this issue:
1: go to  issue #468 
2: download malloc.c
3: copy malloc.c to arduino-1.0.1/hardware/arduino/cores/arduino

Restart the Arduino IDE!


