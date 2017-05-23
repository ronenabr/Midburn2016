#include <FastLED.h>
#include "strips.h"
#include "operation.h"

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG

#define NUM_LEDS    50
CRGB leds[NUM_LEDS];

using uint = unsigned int;
using uint8 = unsigned char;

constexpr uint BRIGHTNESS = 100;
#define FRAMES_PER_SECOND  60

Strip strip1;
Strip strip2;
Circle f1;
Circle f2;
CHSV color = rgb2hsv_approximate(CRGB::Blue);

void setup() {
    delay(2000); // 3 second delay for recovery
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(164);
    Serial.begin(115200);

  strip1.first = 0;
  strip1.last = NUM_LEDS; 
  
  strip2.last = NUM_LEDS/2;
  strip2.first = NUM_LEDS;

  f1 = Circle{.strip = strip1, .base_color = color, .leds = leds, .step=0};
  f2 = Circle{.strip = strip2, .base_color = color, .leds = leds, .step=0};


}

void set_color(uint first, uint last, CRGB color)
{
  last = min(last, NUM_LEDS);
  for (uint i=first; i<last; i++)
  {
    leds[i] = color; 
  }
}

void loop() {
  set_color(0, NUM_LEDS, CRGB(0,0,0));
  //FastLED.show();

  

  for (int i=0; i<6000; i++)
  {
      f1.do_step();
      //f2.do_step();
      FastLED.show();
      delay(120);
  }

  //Flow. Changing color
  // while(1)
  // {
  //   for (uint i=0; i < 512; i++) 
  //   {
  //     colorflow(0, NUM_LEDS, color, i);  
  //     FastLED.show();
  //     delay(60);
  //   }
  // }

  // Rainbow
  // for (uint t=0; t<256; ++t)
  // {
  //   color.h+=2; 
  //   CHSV current = color;
  //   for (uint i=0; i<NUM_LEDS; ++i) {
  //     current.h += 5; 
  //     leds[i] = current; 
  //   }
  //   FastLED.show();
  //   delay(50);

  // }
  

  //DELTA moving. Can change v or h or s
  // while(1) 
  // {
  //   color.h+=5;
    // for (uint offset=0; offset<NUM_LEDS; ++offset) {
    //   for (uint i=0; i<NUM_LEDS; ++i) {
    //       leds[i] = 0; 
    //       if ((i-offset)%10 == 0){
    //         CHSV current = color; 
    //         current.v = 256 * (NUM_LEDS - offset - i)/NUM_LEDS;
    //         // current.s = NUM_LEDS - offset - i;
    //         leds[i] = current; 
    //       }
    //   }
  //     FastLED.show();
  //     delay(60); 
  //   }
  // }

  //All colored. Shifting. 
  // int delta_color = 256/NUM_LEDS; 
  // color.s = 255; 
  // for (int time = 0; time < NUM_LEDS; time++) {
  //   color.h+=delta_color/2; 
  //   for (int i=0; i< NUM_LEDS; i++) {
  //     CHSV current = color; 
  //     current.h += i*delta_color;; 
  //     leds[i] = current; 
  //     if ((i-time)%10 == 0) 
  //     {
  //       leds[i] = 0; 
  //     }
  //   }
  //   FastLED.show();
  //   delay(60);
  // }


}
