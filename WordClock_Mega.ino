/*

 */
#include "FastLED.h"

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define NUM_LEDS 6
#define DATA_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];



void setup() {

      // Die WordClock_Mega benutzt LED-Stripes aus der NeoPixel-Serie
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {

//  leds[1] = CRGB::Red;
//  leds[2] = CRGB::Red;
//  leds[3] = CRGB::Red;

    for (int j=0; j<7; j++)
    {
        for (int i=0; i<=NUM_LEDS; i++)
        {
            if (i > 0)
            {
              leds[i-1] = 0;
            }

            if (i < NUM_LEDS)
            {
                switch (j) {
                  case 0 :
                    leds[i] = CRGB::Yellow;
                  break;
                  case 1 :
                    leds[i] = CRGB::Aqua;
                  break;
                  case 2 :
                    leds[i] = CRGB::Red;
                  break;
                  case 3 :
                    leds[i] = CRGB::Purple;
                  break;
                  case 4 :
                    leds[i] = CRGB::Green;
                  break;
                  case 5 :
                    leds[i] = CRGB::Blue;
                  break;
                  default :
                    leds[i] = CRGB::White;

                }
            }

            FastLED.show();

            delay(100);
        }
    }


    int k=0, l=0;

    while (true)
    {
      leds[k] = 0;

      k = (int) random(0, NUM_LEDS);
      l = (int) random(0, 7);

      switch (l)
      {
        case 0 :
          leds[k] = CRGB::Yellow;
        break;
        case 1 :
          leds[k] = CRGB::Aqua;
        break;
        case 2 :
          leds[k] = CRGB::Red;
        break;
        case 3 :
          leds[k] = CRGB::Purple;
        break;
        case 4 :
          leds[k] = CRGB::Green;
        break;
        case 5 :
          leds[k] = CRGB::Blue;
        break;
        default :
          leds[k] = CRGB::White;

      }
      FastLED.show();

      delay(500);
    }
}

