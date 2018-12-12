#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN1 0
#define PIN2 2

int j=0;
int i=0;
int q=0;
int k=0;
boolean theateroff = false;
int led1 = 0;
int led2 = 1;

// Parameter 1 = number of pixels in strip11
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW product
//eerste ledstrip1 (met 5 kabels)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(72, PIN1, NEO_GRB + NEO_KHZ800);
//tweede ledstrip1 (drie kabels)
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(72, PIN2, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  /*// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code*/

  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  
  strip2.show(); // Initialize all pixels to 'off'  
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  /*colorWipe(strip1.Color(50, 0, 0), 5); // Red
  colorWipe(strip1.Color(0, 55, 0), 5); // Green
  colorWipe(strip1.Color(0, 0, 55), 5); // Blue
  colorWipe(strip1.Color(0, 0, 0), 50); // White RGBW*/
  // Send a theater pixel chase in...
  /*theaterChase(strip1.Color(50, 50, 50), 50, 1); // White
  theaterChase(strip1.Color(50, 0, 0), 50, 1); // Red
  theaterChase(strip1.Color(0, 0, 50), 469, 1); // Blue

  /*colorWipe(strip2.Color(50, 0, 0), 5); // Red
  colorWipe(strip2.Color(0, 55, 0), 5); // Green
  colorWipe(strip2.Color(0, 0, 55), 5); // Blue
  colorWipe(strip2.Color(0, 0, 0), 50); // White RGBW*/
  // Send a theater pixel chase in...
  /*theaterChase(strip2.Color(50, 50, 50), 50, 2); // White
  theaterChase(strip2.Color(50, 0, 0), 50, 2); // Red
  theaterChase(strip2.Color(0, 0, 50), 469, 2); // Blue
  //rainbow(50);
  rainbowCycle(50,1);
  rainbowCycle(50,2);
  /*theaterChaseRainbow(50);
    loopledje();*/
  //ifs voor de rainbow
  if(j >= 256*5) {j=0;}
  if(q >= 3) {q=0;}
  if(i >= strip1.numPixels()) {
    i=0;
    if(led1==0 and led2==0){
      strip1.show();
      strip2.show();
      delay(50); //rainbowdelay
    } 
    j++;
  }

  //ifs voor de theaterchase
  
  if(k >= strip1.numPixels()) {
    k=0;
    if(led1==1 || led2==1){
      strip1.show();
      strip2.show();
      if(theateroff==true){q++;} 
      else{delay(50);}
      theateroff = !theateroff;
    }
  }
  
  //welke functie moet er worden uitgevoerd voor de 1e strip?
  switch(led1) {
    case 0: //rainbow
      //Kleuren uit functie halen en de kleur van de leds setten
      strip1.setPixelColor(i, rainbow(j,i));
     break;
    case 1: //theaterchase
      if(theateroff == false) {
        strip1.setPixelColor(k+q,0,50,0);
      }
      else {strip1.setPixelColor(k+q,0,0,0);}
    break;
    default:
      strip1.setPixelColor(i,0,0,0);
    break;
  }
  //=======================================================
  //welke functie moet er worden uitgevoerd voor de 2e strip?
  switch(led2) {
    case 0://rainbow
      //Kleuren uit functie halen en de kleur van de leds setten
      strip2.setPixelColor(i, rainbow(j,i));
     break;
    case 1://theaterchase
      if(theateroff == false) {
        strip2.setPixelColor(k+q,50,0,0);
      }
      else {strip2.setPixelColor(k+q,0,0,0);}
    break;
    default:
      strip2.setPixelColor(i,0,0,0);
    break;
  }
    
  //=======================================================

  k=k+3;
  i++;
}

int rainbow(int j, int i) {
  //uint16_t i, j;

  //for(j=0; j<256; j++) {
    //for(i=0; i<strip1.numPixels(); i++) {
    //return Wheel((i+j) & 255);
    return Wheel(((i * 256 / strip1.numPixels()) + j) & 255);
      //rainbowLijst = {i, Wheel((i+j) & 255)};
    //}
    //strip1.show();
    //delay(wait);
  //}
}


void loopledje() {
  uint16_t start = 300;
  for(uint16_t j=0; j<strip1.numPixels(); j++) {
    double k=0.0001;
    for(uint16_t i=start; i>j; i--) {
      //k=k+0.25;
      strip1.setPixelColor(i,125,0,125);
      strip1.show();
      delay(k);
      strip1.setPixelColor(i,0,0,0);
    }
    strip1.setPixelColor(j,0,125,125); 
    strip1.setPixelColor(start,125,125,0); 
    start = start-1;
    k=k+0.5;
  }
  strip1.Color(0,0,0);
}


//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int q=0; q < 3; q++) {
      for (uint16_t k=0; k < strip1.numPixels(); k=k+3) {
        strip1.setPixelColor(k+q, c);    //turn every third pixel on
        strip2.setPixelColor(k+q, strip2.Color(0, 0, 50));
      }
      strip1.show();
      strip2.show();

      delay(wait);

      for (uint16_t k=0; k < strip1.numPixels(); k=k+3) {
        strip1.setPixelColor(k+q, 0);        //turn every third pixel off
        strip2.setPixelColor(k+q, 0); 
      }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip1.show();

      delay(wait);

      for (uint16_t i=0; i < strip1.numPixels(); i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
