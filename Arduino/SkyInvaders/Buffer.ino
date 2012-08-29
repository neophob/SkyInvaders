void initAnimationMode() {
 //todo 
}

void loopAnimationMode() {
  switch(oscMode) {
     case MODE_STATIC_COLOR:
       for (unsigned int i=0; i < strip.numPixels(); i++) {
         strip.setPixelColor(i, staticColor);
       }
       break;
       
     case MODE_COLOR_FADE:
       break;
       
     case MODE_SERVER_IMAGE:
       break;
  }
}



