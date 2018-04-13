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
#include <Wire.h>


// Defines zur Pin-Zuordnung für 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8

// Defines für Neopixel-LED-Stripe
#define NUM_LEDS 144        // Anzahl der verwendeten LEDs
#define DATA_PIN 6        // Data-Pin des LED-Stripes liegt auf Arduino-Pin 6



// Initialisierung LED-Stripe
CRGB leds[NUM_LEDS];      // Initialisierung  LED-Stripe

int i;

char strTime[10], strPrevTime[10];
char strDate[12], strPrevDate[12];

// Initialisierung TFT-Display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);


CRGB led_col = CRGB::Red;


void set_leds (CRGB *, int, int, CRGB);

char my_buffer[25];

volatile boolean receiveFlag = false;




void setup() {
  byte error, address;
  int nDevices;

  // Die WordClock_Mega benutzt LED-Stripes aus der NeoPixel-Serie
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  /* die serielle Ausgabe initialisieren */
  Serial.begin(9600);
  Serial.println("Initialisierung");

  // Use this initializer if you're using a 1.8" TFT

  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event


  /* TFT-Display Grundeinstellungen setzen */
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLUE);
  tft.setFont(&FreeSansOblique18pt7b);
  tft.setTextColor(ST7735_YELLOW);

  i = 0;

  Serial.println("Initialisierung abgeschlossen");

  strcpy (strTime, "");
  strcpy (strPrevTime, "");
}



void loop() {

  int nHour, nAM_or_PM, nMinute, nFiveMinutes;
  int led_nr;

  char strWeekday[2], strHour[3], strMinute[3];

  int fb_code;


  if (receiveFlag) {

    receiveFlag = false;

    /* Zeit, Datum, Wochentag und Fernbedienungscode aus Buffer extrahieren */
    strncpy(strTime, my_buffer, 8);
    strTime[8] = 0;
    strncpy(strDate, my_buffer+9, 10);
    strDate[10] = 0;
    strncpy(strWeekday, my_buffer+20, 1);
    strWeekday[1] = 0;
    fb_code = atoi(my_buffer+22);

/*  Debugging-Ausgabe
    Serial.print("-->");
    Serial.print(strTime);
    Serial.print("<--->");
    Serial.print(strDate);
    Serial.print("<--->");
    Serial.print(strWeekday);
    Serial.print("<--->");
    Serial.print(fb_code);
    Serial.println("<--");
 */

    /* Wechsel der Uhrzeit */
    if (strcmp(strTime, strPrevTime) != 0)
    {

      /* Anzeige Uhrzeit */
      strncpy(strHour, strTime, 2);
      strHour[2] = 0;
      strncpy(strMinute, strTime + 3, 2);
      strMinute[2] = 0;

      nHour   = atoi(strHour);
      nMinute = atoi(strMinute);


      for (led_nr=0; led_nr<NUM_LEDS; led_nr++)
      {
        leds[led_nr] = 0;
      }


      set_leds (leds, 1, 2, CRGB::Red); // ES
      Serial.print("Es ");


      switch (nMinute % 5) {
        case 1:
        case 2:
          set_leds (leds,  4,  6, CRGB::Red); // WAR
          set_leds (leds, 25, 30, CRGB::Red); // GERADE
          Serial.print("war gerade ");
          break;
        case 3:
        case 4:
          set_leds (leds,  7,  9, CRGB::Red); // IST
          set_leds (leds, 13, 18, CRGB::Red); // GLEICH
          Serial.print("ist gleich ");
          break;
        default:
          set_leds (leds,  7,  9, CRGB::Red); // IST
          set_leds (leds, 32, 36, CRGB::Red); // GENAU
          Serial.print("ist genau ");
          break;
      }

      nFiveMinutes = int((nMinute + 2)/5);

      switch (nFiveMinutes) {
        case 1:
        case 5:
        case 7:
        case 11:
          set_leds (leds, 45, 48, CRGB::Red); // FÜNF
          Serial.print("fuenf ");
          break;
        case 2:
        case 10:
          set_leds (leds, 49, 52, CRGB::Red); // ZEHN
          Serial.print("zehn ");
          break;
        case 3:
        case 9:
          set_leds (leds, 37, 43, CRGB::Red); // VIERTEL
          Serial.print("viertel ");
          break;
        case 4:
        case 8:
          set_leds (leds, 54, 60, CRGB::Red); // ZWANZIG
          Serial.print("zwanzig ");
          break;
      }


      switch (nFiveMinutes) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
          set_leds (leds, 61, 64, CRGB::Red); // NACH
          Serial.print("nach ");
          break;
        case 5:
        case 8:
        case 9:
        case 10:
        case 11:
          set_leds (leds, 65, 67, CRGB::Red); // VOR
          Serial.print("vor ");
          break;
      }

      switch (nFiveMinutes) {
        case 5:
        case 6:
        case 7:
          set_leds (leds, 69, 72, CRGB::Red); // HALB
          Serial.print("halb ");
          break;
      }

      nAM_or_PM = 0;
      if (nMinute > 22) nHour++;
      if (nHour   > 12)
      {
        nHour = nHour - 12;
        nAM_or_PM = 1;
      }
      switch (nHour) {
        case 1:
          set_leds (leds, 75, 78, CRGB::Red); // EINS
          Serial.print("Ein ");
          break;
        case 2:
          set_leds (leds, 101, 104, CRGB::Red); // ZWEI
          Serial.print("Zwei ");
          break;
        case 3:
          set_leds (leds, 73, 76, CRGB::Red); // DREI
          Serial.print("Drei ");
          break;
        case 4:
          set_leds (leds, 97, 100, CRGB::Red); // VIER
          Serial.print("Vier ");
          break;
        case 5:
          set_leds (leds, 87, 90, CRGB::Red); // FÜNF
          Serial.print("Fünf ");
          break;
        case 6:
          set_leds (leds, 115, 119, CRGB::Red); // SECHS
          Serial.print("Sechs ");
          break;
        case 7:
          set_leds (leds, 78, 83, CRGB::Red); // SIEBEN
          Serial.print("Sieben ");
          break;
        case 8:
          set_leds (leds, 105, 108, CRGB::Red); // ACHT
          Serial.print("Acht ");
          break;
        case 9:
          set_leds (leds, 110, 113, CRGB::Red); // NEUN
          Serial.print("Neun ");
          break;
        case 10:
          set_leds (leds, 121, 124, CRGB::Red); // ZEHN
          Serial.print("Zehn ");
          break;
        case 11:
          set_leds (leds, 85, 87, CRGB::Red); // ELF
          Serial.print("Elf ");
          break;
        default:
          set_leds (leds, 92, 96, CRGB::Red); // ZWÖLF
          Serial.print("Zwoelf ");
      }

      set_leds (leds, 126, 128, CRGB::Red); // UHR
      Serial.print("Uhr ");

      if (nAM_or_PM == 0)
      {
        set_leds (leds, 130, 132, CRGB::Red); // VOR
        Serial.print("vor Mittag");
      }
      else
      {
        set_leds (leds, 133, 136, CRGB::Red); // NACH
        Serial.print("nach Mittag");
      }

      set_leds (leds, 139, 144, CRGB::Red); // MITTAG

      Serial.println("<--");
      FastLED.show();





      /* allgemeine Einstellungen TFT-Display */
      tft.setTextSize(1);
      tft.setTextColor(ST7735_YELLOW);
      tft.setFont(&FreeSansOblique18pt7b);

      /* Ausgabe der Uhrzeit auf dem TFT-Display*/
      int timeString_pos[8] = {10, 29, 48, 58, 77, 96, 106, 125};

      for (int j=0; j<8; j++)
      {

        if (strTime[j] != strPrevTime[j])
        {
          tft.setCursor(timeString_pos[j], 40);
          tft.setTextColor(ST7735_BLUE);
          tft.println(strPrevTime[j]);

          tft.setCursor(timeString_pos[j], 40);
          tft.setTextColor(ST7735_YELLOW);
          tft.println(strTime[j]);
        }
      }

      strcpy(strPrevTime, strTime);
    }


    /* Wechsel des Datums */
    if (strcmp(strDate, strPrevDate) != 0)
    {

      tft.setFont(&FreeSansOblique12pt7b);

      tft.fillRect(21,63,121,23, ST7735_BLUE);

      switch (atoi (strWeekday)) {
        case 0:
          tft.setCursor(41, 80);
          tft.println("Montag");
          break;
        case 1:
          tft.setCursor(34, 80);
          tft.println("Dienstag");
          break;
        case 2:
          tft.setCursor(35, 80);
          tft.println("Mittwoch");
          break;
        case 3:
          tft.setCursor(19, 80);
          tft.println("Donnerstag");
          break;
        case 4:
          tft.setCursor(44, 80);
          tft.println("Freitag");
          break;
        case 5:
          tft.setCursor(33, 80);
          tft.println("Samstag");
          break;
        case 06:
          tft.setCursor(36, 80);
          tft.println("Sonntag");
          break;
      }


      tft.setTextColor(ST7735_BLUE);
      tft.setCursor(20, 113);
      tft.println(strPrevDate);

      tft.setTextColor(ST7735_YELLOW);
      tft.setCursor(20, 113);
      tft.println(strDate);

      strcpy(strPrevDate, strDate);
    }


/*
    Serial.println (strTime);

    for (led_nr=0; led_nr<NUM_LEDS; led_nr++)
    {
      leds[led_nr] = 0;
    }

    set_leds (leds, 1, 2, CRGB::Red);   // Es

    Serial.print ("Es ");

    switch (minute % 5) {
      case 0:
        set_leds (leds, 4, 6, CRGB::Red);     // ist
        set_leds (leds, 13, 17, CRGB::Red);   // genau
        Serial.println ("ist genau ");
        break;
      case 1:
      case 2:
        set_leds (leds, 7, 9, CRGB::Red);     // war
        set_leds (leds, 18, 23, CRGB::Red);   // gerade
        Serial.println ("war gerade ");
        break;
      case 3:
      case 4:
        set_leds (leds, 4, 6, CRGB::Red);    // ist
        set_leds (leds, 25, 30, CRGB::Red);  // gleich
        Serial.println ("ist gleich");
        break;
    }


//    set_leds (leds, 17, 21, CRGB::Green);

    set_leds (leds, 35, 42, CRGB::Blue);

//    set_leds (leds, 76, 78, CRGB::Red);

*/



/*
  delay (1000);
  */
  }


}



void set_leds (CRGB *my_leds, int nr_start, int nr_end, CRGB n_col)
{

  for (int led_nr=nr_start-1; led_nr<nr_end; led_nr++)
  {
      *(my_leds + led_nr) = n_col;
  }
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int j=0;

  while( Wire.available()){
    my_buffer[j++] = Wire.read();
  }
  my_buffer[j] = 0;

  receiveFlag = true;
}



/*
  switch (x)
  {
    case 5:
      led_col = CRGB::Red;
      tft.fillScreen(ST7735_RED);
      break;
    case 6:
      led_col = CRGB::Green;
      tft.fillScreen(ST7735_GREEN);
     break;
    case 7:
      led_col = CRGB::Blue;
      tft.fillScreen(ST7735_BLUE);
      break;

  }
*/


