// Copyright [2024] CC0 1.0 Universal
// Initial contributor: Kevin Prichard https://github.com/Kevin-Prichard
// Color bars display for Adafruit NeoPixel / WS2812B strips

#include <stdio.h>
#include <stdlib.h>

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#include "HeadbandColors.h"

// Serial Debugging
// What to send back to help debug this program
#define DEBUG_SETUP 0x01         // .... ...1  setup()
#define DEBUG_NEWBAR 0x02        // .... ..1.  newBar()
#define DEBUG_GETCOLOR 0x04      // .... .1..  getUnrelatedColor()
#define DEBUG_COLORSMERGED 0x08  // .... 1...  loop() - colors merged
#define DEBUG_POSCHANGED 0x10    // ...1 ....  loop() - position changed
#define DEBUG_POINTDIST 0x20     // ..1. ....  pointDistance()
#define DEBUG_COLORDIST 0x40     // .1.. ....  colorDistance()
#define DEBUG_LOOPITERS 0x80     // 1... ....  loop()

// Serial debugging: default is all off
#define SERIAL_DEBUG 0

// Serial debugging: full chat
// Careful! This will likely pin your CPU & keep it from completing setup()
// Enable only what you need.
/*
#define SERIAL_DEBUG (DEBUG_SETUP | DEBUG_NEWBAR | DEBUG_GETCOLOR | \
                      DEBUG_COLORSMERGED | DEBUG_POSCHANGED | \
                      DEBUG_POINTDIST | DEBUG_COLORDIST)
*/

#define RESTART_AT_LOOPCOUNT 25000

#define DEBUG_MAX_LINE 400

// Number of NeoPixels attached to the ucontroller
#define NUM_PIXELS 144

// Arduino pin used to drive the NeoPixel array
#define PIN 6

// Minimum color byte value
#define MIN_LUMINOSITY 4

// Dim colors to this fraction
// eventually obtain this value from a rotary encoder or button
#define DIMMER 1.0

// New color bars must have color that is at least this fraction of Euclidean
// distance away from any other color currently in use
#define COLOR_DISTANCE_REQ 0.33333

// Maximum possible euclidean distance between brightest and darkest colors
#define MAX_COLOR_DISTANCE sqrt(pow(256, 2) * 3)

// minimum number of color bars per session
#define MIN_BARS 3

// maxium number of color bars per session
#define MAX_BARS 8

// minimum color bar length
#define MIN_BAR_LENGTH 4

// maximum color bar length
#define MAX_BAR_LENGTH 16

// minimum color bar speed per loop()
#define MIN_SPEED_PIXSEC 0.25

// maximum color bar speed per loop()
#define MAX_SPEED_PIXSEC 2.0

// minimum alpha / transparency for a color bar
#define MIN_TRANSPARENCY 0.1

// maximum alpha of a color bar
#define MAX_TRANSPARENCY 1.0

// minimum lifespan of a color bar as the number of trips around the strip
#define MIN_LOOPS_LIFESPAN 1

// maximum trips around the strip
#define MAX_LOOPS_LIFESPAN 5

// Data about each color bar, which are created in newBar()
struct ColorBar {
  uint32_t length;     // number of pixels in length
  uint32_t startPos;   // start position on the LED strip
  float curPos;        // current position on the strip
  PixelColor color;    // color of the bar
  float speed;         // velocity of the bar (currently unchanging)
  bool direction;      // direction, forwards or backwards
  float alpha;         // transparency
  float taperHead;     // number of pixels to fade the colorbar's head
  float taperTail;     // number of pixels to fade the tail
  uint32_t lifeSpan;   // how long this colorbar lives (unimplemented)
} colorBars[MAX_BARS];


// Difference between two colors
inline float pointDistance(
    PixelColor c1, PixelColor c2, PixelColor mask, int base) {
  int c1m = (c1 & mask) >> base;
  int c2m = (c2 & mask) >> base;
  int cdiff = c1m - c2m;

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_POINTDIST)
  char buf[DEBUG_MAX_LINE];
  snprintf(buf, DEBUG_MAX_LINE, "c1m: %d, c2m: %d, cdiff: %d", c1m, c2m, cdiff);
  Serial.println(buf);
#endif

  return cdiff;
}

// Euclidean distance between two colors in RGB 3-space
int colorDistance(PixelColor c1, PixelColor c2) {
  float red =   pointDistance(c1, c2, 0x00FF0000, 16);
  float green = pointDistance(c1, c2, 0x0000FF00, 8);
  float blue =  pointDistance(c1, c2, 0x000000FF, 0);
  float result = sqrt(pow(red, 2.0) + pow(green, 2.0) + pow(blue, 2.0));

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_COLORDIST)
  char buf[100], fred[40], fgreen[40], fblue[40], fresult[40];
  dtostrf(red, 10, 3, &fred[0]);
  dtostrf(green, 10, 3, &fgreen[0]);
  dtostrf(blue, 10, 3, &fblue[0]);
  dtostrf(result, 10, 3, &fresult[0]);
  snprintf(buf, DEBUG_MAX_LINE, 
           "===== red: %s, green: %s, blue: %s, result: %s =====\n",
           fred, fgreen, fblue, fresult);
  Serial.println(buf);
#endif

  return result;
}

int barCount;
unsigned long loopIterCount;

PixelColor getUnrelatedColor(char forThisBar) {
  // Find a color in pixelColors table that is at least COLOR_DISTANCE_REQ
  //     away from any other color in use by a live colorbar
  int maxAttempts = 1 << 10, attempts = 0;
  PixelColor colorCandidate = 0, colorInUse = 0;
  int barNo = 0;

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
  char buf[DEBUG_MAX_LINE], pctMaxDist[15], colDistReq[15];
#endif

  randomSeed(analogRead(0));

  while (attempts++ < maxAttempts) {

    // Obtain a hopefully random value to seed the pseudo-RNG
    colorCandidate = pixelColors[random(pixelColorsCount)];
    int safeDistance = 0;
    for (barNo = 0; barNo < forThisBar; barNo++) {
      colorInUse = colorBars[barNo].color;
      int thisColDist = 0, farEnough = 0;

      if (colorInUse > 0) {
        thisColDist = colorDistance(colorCandidate, colorInUse);
        farEnough = thisColDist / MAX_COLOR_DISTANCE >= COLOR_DISTANCE_REQ;
      } else {
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
        Serial.println("ColorInUse == 0 ***********************************************");
#endif
        farEnough = 1;
      }
      if ( farEnough ) {
        ++safeDistance;
      }

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
        dtostrf(thisColDist / MAX_COLOR_DISTANCE, 12, 4, &pctMaxDist[0]);
        dtostrf(COLOR_DISTANCE_REQ, 12, 4, &colDistReq[0]);
        snprintf(buf, DEBUG_MAX_LINE,
          "attempts: %d, barNo: %d (barCount: %d, forThisBar: %d),"
          "colorCandidate: %lx, colorInUse: %lx,"
          "thisColDist: %d, farEnough: %d"
          ", thisColDist / MAX_COLOR_DISTANCE = %s >= COLOR_DISTANCE_REQ(%s)\n", 
          attempts, barNo, barCount, forThisBar,
          colorCandidate, colorInUse,
          thisColDist, farEnough,
          pctMaxDist, colDistReq);
        Serial.print(buf);
#endif

      if ( !farEnough ) {
#if (SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
        Serial.println("!farEnough BREAK %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");
#endif
        break;
      }
    }

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
    snprintf(buf, DEBUG_MAX_LINE, "safeDistance[%d] >= forThisBar[%d] = %d", safeDistance, forThisBar, safeDistance >= forThisBar);
    Serial.println(buf);
#endif

    if ((safeDistance >= forThisBar) || (forThisBar == 0)) {
      break;
    }
  }
  return colorCandidate;
}

void newBar(ColorBar *b, int barNo) {
  b->length = MIN_BAR_LENGTH + random(MAX_BAR_LENGTH - MIN_BAR_LENGTH);
  b->startPos = random(NUM_PIXELS);
  b->curPos = b->startPos;
  b->color = 0;
  b->speed = MIN_SPEED_PIXSEC + (random(256) / 256.0) *
            (MAX_SPEED_PIXSEC - MIN_SPEED_PIXSEC);
  b->direction = random(2) >= 1 ? true : false;
  b->alpha = random(256) / 256.0;
  b->taperHead = 0.0;
  b->taperTail = 0.0;
  b->lifeSpan = MIN_LOOPS_LIFESPAN +
               random(MAX_LOOPS_LIFESPAN - MIN_LOOPS_LIFESPAN);

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_NEWBAR)
  char buf[DEBUG_MAX_LINE];
  char fcur[20], fspeed[20];
  dtostrf(b->curPos, 12, 4, fcur);
  dtostrf(b->speed, 12, 4, fspeed);
  snprintf(buf, DEBUG_MAX_LINE,
    "Bar#%d: length=%lu, start=%lu, cur=%s, "
    "color=%8lx, speed=%s, dir=%d, life=%lu\n",
    barNo,
    b->length,
    b->startPos,
    fcur,
    b->color,
    fspeed,
    b->direction,
    b->lifeSpan);
  Serial.print(buf);
#endif

}


void setup() {
  loopIterCount = 0;
  pinMode(LED_BUILTIN, OUTPUT);
  randomSeed(analogRead(0));

  barCount = MIN_BARS + random(MAX_BARS - MIN_BARS);
  // delay(1000);
  Serial.begin(115200);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  // Adafruit Trinket 5V 16 MHz.
  clock_prescale_set(clock_div_1);
#endif

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_SETUP)
  char buf[DEBUG_MAX_LINE];
  snprintf(buf, DEBUG_MAX_LINE, 
    "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
    barCount,
    SERIAL_DEBUG & DEBUG_SETUP,
    SERIAL_DEBUG & DEBUG_NEWBAR,
    SERIAL_DEBUG & DEBUG_GETCOLOR,
    SERIAL_DEBUG & DEBUG_COLORSMERGED,
    SERIAL_DEBUG & DEBUG_POSCHANGED,
    SERIAL_DEBUG & DEBUG_POINTDIST,
    SERIAL_DEBUG & DEBUG_COLORDIST);

  snprintf(buf, DEBUG_MAX_LINE,
    "%dX\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
    SERIAL_DEBUG,
    DEBUG_SETUP,
    DEBUG_NEWBAR,
    DEBUG_GETCOLOR,
    DEBUG_COLORSMERGED,
    DEBUG_POSCHANGED,
    DEBUG_POINTDIST,
    DEBUG_COLORDIST);
  Serial.print(buf);
#endif

  for (int barNo = 0; barNo < barCount; barNo++) {
    newBar(&colorBars[barNo], barNo);
  }

  for (int barNo = 0; barNo < barCount; barNo++) {
    colorBars[barNo].color = getUnrelatedColor(barNo);
  }

  // init LED strip object
  pixels.begin();
}


void loop() {
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG > 0)
      char buf[DEBUG_MAX_LINE];
#endif

  pixels.clear();

  for (int barNo = 0; barNo < barCount; barNo++) {
    ColorBar* bar = &colorBars[barNo];
    for (uint32_t barPix = bar->curPos;
         barPix < bar->curPos + bar->length;
         barPix++) {
      uint32_t curPixColor = pixels.getPixelColor(barPix % NUM_PIXELS);
      unsigned char red, green, blue;
      uint32_t newColor;
      pixels.setPixelColor(
        barPix % NUM_PIXELS,
        newColor = pixels.Color(
          red =   ((((bar->color & (uint32_t)0x00FF0000) >> 16) * bar->alpha) +
                  (((curPixColor & (uint32_t)0x00FF0000) >> 16) *
                    (1.0 - bar->alpha))) / 2 * DIMMER,
          green = ((((bar->color & (uint32_t)0x0000FF00) >>  8) * bar->alpha) +
                  (((curPixColor & (uint32_t)0x0000FF00) >>  8) *
                    (1.0 - bar->alpha))) / 2 * DIMMER,
          blue =  (((bar->color & (uint32_t)0x000000FF) * bar->alpha) +
                  (curPixColor & (uint32_t)0x000000FF) *
                    (1.0 - bar->alpha))  / 2 * DIMMER));


#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_COLORSMERGED)
      snprintf(buf, DEBUG_MAX_LINE,
        "bar#%d \tpix#%lu \tpix#%lu \tred:%X "
        "\tgreen:%X \tblue:%X \tcolor:%lX\n",
        barNo,
        barPix,
        (uint32_t)(barPix % NUM_PIXELS),
        red,
        green,
        blue,
        newColor);
      Serial.print(buf);
#endif
    }

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_POSCHANGED)
    Serial.print(barNo);
    Serial.print("\t");
    Serial.print(bar->curPos);
    Serial.print("\t");
#endif

    bar->curPos = bar->curPos + bar->speed * (bar->direction ? 1 : -1);
    if (bar->curPos > NUM_PIXELS) {
      bar->curPos = 0;
    }
    if (bar->curPos < 0) {
      bar->curPos = NUM_PIXELS;
    }

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_POSCHANGED)
    Serial.print("curPos: ");
    Serial.print(bar->curPos);
    Serial.print("\n");
#endif
  }

  // Send the updated pixel colors to the hardware.
  pixels.show();
  ++loopIterCount;
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_LOOPITERS)
  if ( (loopIterCount % min(RESTART_AT_LOOPCOUNT, 1000)) == 0 ) {
    snprintf(buf, DEBUG_MAX_LINE, "loopIterCount: %lu", loopIterCount);
    Serial.println(buf);
  }
  if ( loopIterCount >= RESTART_AT_LOOPCOUNT) {
    setup();
  }
#endif

}
