#include <FastLED.h>


#define LED_PIN_A     3
#define LED_PIN_B     6
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS    31

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 20

bool gReverseDirection = false;

CRGB leds_A[NUM_LEDS];
CRGB leds_B[NUM_LEDS];

unsigned long time = 0;
unsigned long ten_secs = 1000*10; 
unsigned long minute = 1000*60; 

void setup() {
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN_A, COLOR_ORDER>(leds_A, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN_B, COLOR_ORDER>(leds_B, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  	Serial.begin(57600);
	Serial.println("resetting");

}

int state = 0; 
int state_t = 0; 
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
unsigned long span;

int state_counter = 0; 
unsigned short last_state[10];

void loop()
{
  
  


   if (state_t == 0) {
     CoolCircle(leds_B);
     off(leds_A); 
  }
  else {
    Fire2012(leds_A); // run simulation frame
   //   fill_rainbow( leds_B, NUM_LEDS, gHue, 7); // run simulation frame
   juggle(leds_B);
  }

  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);

    //Serial.print(span);
      //  Serial.print(" , ");

    //Serial.println(time);
    //Serial.print(state);
    //Serial.print(" , ");
    //Serial.println(state_t);

    state = 1; 
    for (state_counter=0; state_counter<50; state_counter++)
    {
     int buttonState = analogRead(2);
          Serial.println(buttonState); 

     if (buttonState>1000){
       state = 1 * state; 
     }
     else {
       state = 0; 
     }
    }
    if (state == 1 && time == 0)
    {
      time = millis();
    }



  if (time == 0) 
  {
     return; 
  }
  unsigned long span = (millis()-time)/1000;
  if (state == 1 && span < 10)
  {
    state_t = 1; 
  }
  if (state == 1 && span > 10)
  {
    state_t = 0; 
  }
  if (state == 0 && span > 10)
  {
    time = 0;
    state_t = 0; 
  }
  
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012(CRGB * leds)
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}

void CoolCircle(CRGB * leds)
{
    static char heated = 0;
    
    for (int i=0; i < NUM_LEDS; i++) {
      short power = sin8((i+heated)*255/NUM_LEDS);
      power = power*power/512;
      leds[i].r = power; 
      leds[i].g = 0;
      leds[i].b = 0;
    }
    
    heated  = (heated+1) % NUM_LEDS;
}

void off(CRGB * leds)
{
  for (int i=0; i < NUM_LEDS; i++) 
  {
    leds[i].r = 0;
    leds[i].g = 0;
    leds[i].b = 0; 
  }
}

void bpm(CRGB * leds)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle(CRGB * leds) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

