#ifndef OPERATION_H
#define OPERATION_H

#include "strips.h"
#include <FastLED.h>


struct  FlowChangeColor
{
    Strip strip; 
    CHSV base_color; 
    CRGB *  leds; 
    uint step; 


    void flow(uint step, CHSV color) {
        int step_size = strip.step();
        for (uint i=strip.first; i!=strip.last; i+=step_size)
        {
            CHSV current = color; 
            current.v += 256 * (step-step_size*i)/30;
            leds[i] = current;
            Serial.println(leds[i].b); 
        }
    }

    void do_step()
    {
        base_color.h += 1;
        flow(step, base_color);
        step++; 
    }


};



struct  Rainbow
{
    Strip strip; 
    CHSV base_color; 
    CRGB *  leds; 
    uint step; 

    void do_step()
    {
    
        base_color.h+=2; 
        CHSV current = base_color;
        int step_size = strip.step(); 
        for (uint i=strip.first; i!=strip.last; i+=step_size) {
            current.h += 5; 
            leds[i] = current; 
        }
        step++;
    }
};


struct  DynamicRainbow
{
    Strip strip; 
    CHSV base_color; 
    CRGB *  leds; 
    uint step; 

    void do_step()
    {
      int step_size = strip.step(); 
      int length = (strip.last - strip.first) * step_size; 
      int delta_color = 256/length; 
      base_color.h+=delta_color/2; 
        for (uint i=strip.first; i!=strip.last; i+=step_size) {
            CHSV current = base_color; 
            current.h += i*delta_color;; 
            leds[i] = current; 
            if ((i-step)%10 == 0) 
            {
                leds[i] = 0; 
            } 
        }
        step++; 
    }
};


struct  Pulse
{
    Strip strip; 
    CHSV base_color; 
    CRGB *  leds; 
    uint step; 

    void do_step()
    {
        base_color.h+=5;
        int step_size = strip.step(); 
        int length = (strip.last - strip.first) * step_size; 
        for (uint i=strip.first; i!=strip.last; i+=step_size) {
          leds[i] = 0; 
          if ((i*step_size-step)%10 == 0){
            CHSV current = base_color; 
            current.v = 256 * (length - step - i*step_size)/length;
            leds[i] = current; 
          } 
         step++; 
       }
    }
};

struct  Circle
{
    Strip strip; 
    CHSV base_color; 
    CRGB *  leds; 
    uint step; 

    void do_step()
    {        
        base_color.h+=1;
        int step_size = strip.step(); 
        int length = (strip.last - strip.first) * step_size; 
        for (uint i=strip.first; i!=strip.last; i+=step_size) {
            short power = sin8((i*step_size+step)*512/length);
            CHSV current = base_color; 
            current.v = power * power / 512; 
            leds[i] = current; 
        }
        step  = (step+1) % length;
    }
    
};


#endif //OPERATION_H