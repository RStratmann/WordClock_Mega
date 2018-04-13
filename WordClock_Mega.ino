// Includes für TFT-Anzeige
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Fonts/FreeSansOblique18pt7b.h>
#include <Fonts/FreeSansOblique12pt7b.h>

// Include für Neopixel-LED-Stripes
#include "FastLED.h"

// Include für Schnittstellen
#include <SPI.h>


// Defines zur Pin-Zuordnung für 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

// Defines für Neopixel-LED-Stripe
#define NUM_LEDS 6        // Anzahl der verwendeten LEDs
#define DATA_PIN 6        // Data-Pin des LED-Stripes liegt auf Arduino-Pin 6



// Initialisierung LED-Stripe
CRGB leds[NUM_LEDS];      // Initialisierung  LED-Stripe

// Initialisierung TFT-Display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);




void setup() {

      // Die WordClock_Mega benutzt LED-Stripes aus der NeoPixel-Serie
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

        /* die serielle Ausgabe initialisieren */
      Serial.begin(115200);
      Serial.println("Hallo");

      // Use this initializer if you're using a 1.8" TFT
      tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

      tft.setTextWrap(false);
      tft.setRotation(3);

      Serial.println("Initialized");
}



void loop() {

  char clockString[30];



  // put your main code here, to run repeatedly:
  tft.fillScreen(ST7735_BLUE);

  tft.setFont(&FreeSansOblique18pt7b);
  tft.setTextColor(ST7735_YELLOW);

  tft.setCursor(10, 40);
//  tft.setTextSize(2);

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
                    sprintf(clockString, "Gelb");
                  break;
                  case 1 :
                    leds[i] = CRGB::Aqua;
                    sprintf(clockString, "Aqua");
                  break;
                  case 2 :
                    leds[i] = CRGB::Red;
                    sprintf(clockString, "Rot");
                  break;
                  case 3 :
                    leds[i] = CRGB::Purple;
                    sprintf(clockString, "Lila");
                  break;
                  case 4 :
                    leds[i] = CRGB::Green;
                    sprintf(clockString, "Grün");
                  break;
                  case 5 :
                    leds[i] = CRGB::Blue;
                    sprintf(clockString, "Blau");
                  break;
                  default :
                    leds[i] = CRGB::White;
                    sprintf(clockString, "Weiß");

               }
            }

            FastLED.show();

            tft.setCursor(10, 40);
             //  tft.setTextSize(2);

            tft.println(clockString);

            delay(500);
        }

        tft.setTextColor(ST7735_BLUE);
        tft.setCursor(10, 40);
        tft.println(clockString);
        tft.setTextColor(ST7735_YELLOW);

    }


    int k=0, l=0;

    while (true)
    {
      leds[k] = 0;

      k = (int) random(0, NUM_LEDS);
      l = (int) random(0, 7);

      switch (l)
      {
          leds[k] = CRGB::Yellow;
          sprintf(clockString, "Gelb");
        break;
        case 1 :
          leds[k] = CRGB::Aqua;
          sprintf(clockString, "Aqua");
        break;
        case 2 :
          leds[k] = CRGB::Red;
          sprintf(clockString, "Rot");
        break;
        case 3 :
          leds[k] = CRGB::Purple;
          sprintf(clockString, "Lila");
        break;
        case 4 :
          leds[k] = CRGB::Green;
          sprintf(clockString, "Grün");
        break;
        case 5 :
          leds[k] = CRGB::Blue;
          sprintf(clockString, "Blau");
        break;
        default :
          leds[k] = CRGB::White;
          sprintf(clockString, "Weiß");

      }
      FastLED.show();

      tft.setCursor(10, 40);

      tft.println(clockString);


      delay(2500);

      tft.setTextColor(ST7735_BLUE);
      tft.setCursor(10, 40);
      tft.println(clockString);
      tft.setTextColor(ST7735_YELLOW);
    }

}

