void initAnimationMode() {
  switch(oscMode) {
     case MODE_STATIC_COLOR:
       break;
       
     case MODE_COLOR_FADE:
       loadColorSet(oscColorSetNr);
       break;
       
     case MODE_SERVER_IMAGE:
       break;
  }
  
 
}

byte colorFadeOffset=0;

void loopAnimationMode() {
  switch(oscMode) {
     case MODE_STATIC_COLOR:
       for (unsigned int i=0; i < strip.numPixels(); i++) {
         strip.setPixelColor(i, staticColor);
       }
       break;
       
     case MODE_COLOR_FADE:
       for (unsigned int i=0; i < strip.numPixels(); i++) {
         strip.setPixelColor(i, getSmoothColor((colorFadeOffset+i)%255));
       }
       colorFadeOffset++;
       break;
       
     case MODE_SERVER_IMAGE:
       break;
  }
}



