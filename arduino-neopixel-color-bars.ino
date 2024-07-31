// Kevin Prichard
// Color bars display for Adafruit NeoPixel / WS2812B strips
#include <Adafruit_NeoPixel.h>

// What to send back to help debug this program
#define DEBUG_SETUP 0x01        // .... ...1  setup()
#define DEBUG_NEWBAR 0x02       // .... ..1.  newBar()
#define DEBUG_GETCOLOR 0x04     // .... .1..  getUnrelatedColor()
#define DEBUG_COLORSMERGED 0x08 // .... 1...  loop() - colors merged
#define DEBUG_POSCHANGED 0x10   // ...1 ....  loop() - position changed
#define DEBUG_POINTDIST 0x20    // ..1. ....  pointDistance()
#define DEBUG_COLORDIST 0x40    // .1.. ....  colorDistance()

// Serial debugging: all off
#define SERIAL_DEBUG 0

// Serial debugging: full chat (Careful! This will probably pin your device and keep it from completing setup.  Suggest using just what you need.)
// #define SERIAL_DEBUG DEBUG_SETUP | DEBUG_NEWBAR | DEBUG_GETCOLOR | DEBUG_COLORSMERGED | DEBUG_POSCHANGED | DEBUG_POINTDIST | DEBUG_COLORDIST

#include <stdio.h>

#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

// Number of NeoPixels attached to the Arduino
#define NUM_PIXELS 72  // Popular NeoPixel ring size

// Arduino pin used to drive the NeoPixel array
#define PIN 6  // On Trinket or Gemma, suggest changing this to 1

// Instantiate Adafruit NeoPixel handler
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

typedef uint32_t PixelColor;

PixelColor pixelColors[] = {
  // Sorted in contrast order, by the mean of absolute differences between each primary color
  // Low-contrast colors are commented out, to provide a more vivid display
	// /*   0 */  pixels.Color(0,0,0),  /* black */
	// /*   0 */  pixels.Color(105,105,105),  /* dim_gray_/_dim_grey */
	// /*   0 */  pixels.Color(128,128,128),  /* gray_/_grey */
	// /*   0 */  pixels.Color(169,169,169),  /* dark_gray_/_dark_grey */
	// /*   0 */  pixels.Color(192,192,192),  /* silver */
	// /*   0 */  pixels.Color(211,211,211),  /* light_gray_/_light_grey */
	// /*   0 */  pixels.Color(220,220,220),  /* gainsboro */
	// /*   0 */  pixels.Color(245,245,245),  /* white_smoke */
	// /*   0 */  pixels.Color(255,255,255),  /* white */
	// /*   1 */  pixels.Color(255,250,250),  /* snow */
	// /*   2 */  pixels.Color(248,248,255),  /* ghost_white */
	// /*   5 */  pixels.Color(255,245,238),  /* sea_shell */
	// /*   5 */  pixels.Color(245,255,250),  /* mint_cream */
	// /*   5 */  pixels.Color(255,250,240),  /* floral_white */
	// /*   5 */  pixels.Color(240,248,255),  /* alice_blue */
	// /*   5 */  pixels.Color(255,255,240),  /* ivory */
	// /*   5 */  pixels.Color(240,255,255),  /* azure */
	// /*   6 */  pixels.Color(255,240,245),  /* lavender_blush */
	// /*   6 */  pixels.Color(250,240,230),  /* linen */
	// /*   6 */  pixels.Color(230,230,250),  /* lavender */
	// /*   7 */  pixels.Color(253,245,230),  /* old_lace */
	// /*   8 */  pixels.Color(245,245,220),  /* beige */
	// /*  10 */  pixels.Color(47,79,79),  /* dark_slate_gray */
	// /*  10 */  pixels.Color(224,255,255),  /* light_cyan */
	// /*  10 */  pixels.Color(255,255,224),  /* light_yellow */
	// /*  10 */  pixels.Color(255,228,225),  /* misty_rose */
	// /*  10 */  pixels.Color(112,128,144),  /* slate_gray */
	// /*  10 */  pixels.Color(240,255,240),  /* honeydew */
	// /*  11 */  pixels.Color(250,235,215),  /* antique_white */
	// /*  11 */  pixels.Color(255,248,220),  /* corn_silk */
	// /*  11 */  pixels.Color(119,136,153),  /* light_slate_gray */
	// /*  13 */  pixels.Color(250,250,210),  /* light_golden_rod_yellow */
	// /*  14 */  pixels.Color(255,239,213),  /* papaya_whip */
	// /*  15 */  pixels.Color(188,143,143),  /* rosy_brown */
	// /*  15 */  pixels.Color(176,196,222),  /* light_steel_blue */
	// /*  16 */  pixels.Color(216,191,216),  /* thistle */
	// /*  16 */  pixels.Color(255,235,205),  /* blanched_almond */
	// /*  16 */  pixels.Color(255,250,205),  /* lemon_chiffon */
	// /*  18 */  pixels.Color(176,224,230),  /* powder_blue */
	// /*  19 */  pixels.Color(173,216,230),  /* light_blue */
	// /*  19 */  pixels.Color(255,228,196),  /* bisque */
	// /*  21 */  pixels.Color(175,238,238),  /* pale_turquoise */
	// /*  21 */  pixels.Color(95,158,160),  /* cadet_blue */
	// /*  22 */  pixels.Color(238,232,170),  /* pale_golden_rod */
	// /*  22 */  pixels.Color(245,222,179),  /* wheat */
	// /*  23 */  pixels.Color(210,180,140),  /* tan */
	// /*  23 */  pixels.Color(255,218,185),  /* peach_puff */
	// /*  24 */  pixels.Color(255,192,203),  /* pink */
	// /*  24 */  pixels.Color(255,228,181),  /* moccasin */
	// /*  27 */  pixels.Color(189,183,107),  /* dark_khaki */
	// /*  27 */  pixels.Color(85,107,47),  /* dark_olive_green */
	// /*  27 */  pixels.Color(255,222,173),  /* navajo_white */
	// /*  28 */  pixels.Color(255,182,193),  /* light_pink */
	// /*  29 */  pixels.Color(25,25,112),  /* midnight_blue */
	// /*  29 */  pixels.Color(72,61,139),  /* dark_slate_blue */
	// /*  29 */  pixels.Color(222,184,135),  /* burly_wood */
	// /*  30 */  pixels.Color(143,188,143),  /* dark_sea_green */
	// /*  33 */  pixels.Color(240,230,140),  /* khaki */
	// /*  33 */  pixels.Color(135,206,235),  /* sky_blue */
	// /*  36 */  pixels.Color(70,130,180),  /* steel_blue */
	// /*  37 */  pixels.Color(205,92,92),  /* indian_red */
	// /*  37 */  pixels.Color(240,128,128),  /* light_coral */
	// /*  37 */  pixels.Color(233,150,122),  /* dark_salmon */
	// /*  38 */  pixels.Color(135,206,250),  /* light_sky_blue */
	// /*  38 */  pixels.Color(160,82,45),  /* sienna */
	// /*  40 */  pixels.Color(221,160,221),  /* plum */
	// /*  40 */  pixels.Color(139,69,19),  /* saddle_brown */
	// /*  41 */  pixels.Color(165,42,42),  /* brown */
	// /*  42 */  pixels.Color(128,0,0),  /* maroon */
	// /*  42 */  pixels.Color(128,128,0),  /* olive */
	// /*  42 */  pixels.Color(0,128,128),  /* teal */
	// /*  42 */  pixels.Color(0,0,128),  /* navy */
	// /*  43 */  pixels.Color(106,90,205),  /* slate_blue */
	// /*  44 */  pixels.Color(255,160,122),  /* light_salmon */
	// /*  45 */  pixels.Color(250,128,114),  /* salmon */
	// /*  45 */  pixels.Color(100,149,237),  /* corn_flower_blue */
	// /*  46 */  pixels.Color(139,0,0),  /* dark_red */
	// /*  46 */  pixels.Color(102,205,170),  /* medium_aqua_marine */
	// /*  46 */  pixels.Color(0,139,139),  /* dark_cyan */
	// /*  46 */  pixels.Color(0,0,139),  /* dark_blue */
	// /*  47 */  pixels.Color(107,142,35),  /* olive_drab */
	// /*  47 */  pixels.Color(72,209,204),  /* medium_turquoise */
	// /*  47 */  pixels.Color(147,112,219),  /* medium_purple */
	// /*  47 */  pixels.Color(219,112,147),  /* pale_violet_red */
	// /*  47 */  pixels.Color(205,133,63),  /* peru */
	/*  48 */  pixels.Color(178,34,34),  /* firebrick */
	/*  48 */  pixels.Color(46,139,87),  /* sea_green */
	/*  49 */  pixels.Color(244,164,96),  /* sandy_brown */
	/*  51 */  pixels.Color(32,178,170),  /* light_sea_green */
	/*  51 */  pixels.Color(123,104,238),  /* medium_slate_blue */
	/*  53 */  pixels.Color(65,105,225),  /* royal_blue */
	/*  57 */  pixels.Color(184,134,11),  /* dark_golden_rod */
	/*  57 */  pixels.Color(127,255,212),  /* aqua_marine */
	/*  58 */  pixels.Color(255,127,80),  /* coral */
	/*  58 */  pixels.Color(64,224,208),  /* turquoise */
	/*  60 */  pixels.Color(210,105,30),  /* chocolate */
	/*  61 */  pixels.Color(255,99,71),  /* tomato */
	/*  61 */  pixels.Color(60,179,113),  /* medium_sea_green */
	/*  62 */  pixels.Color(218,165,32),  /* golden_rod */
	/*  62 */  pixels.Color(144,238,144),  /* light_green */
	/*  66 */  pixels.Color(0,100,0),  /* dark_green */
	/*  66 */  pixels.Color(152,251,152),  /* pale_green */
	/*  68 */  pixels.Color(154,205,50),  /* yellow_green */
	/*  68 */  pixels.Color(0,0,205),  /* medium_blue */
	/*  68 */  pixels.Color(75,0,130),  /* indigo */
	/*  69 */  pixels.Color(0,206,209),  /* dark_turquoise */
	/*  69 */  pixels.Color(218,112,214),  /* orchid */
	/*  70 */  pixels.Color(34,139,34),  /* forest_green */
	/*  72 */  pixels.Color(238,130,238),  /* violet */
	/*  75 */  pixels.Color(30,144,255),  /* dodger_blue */
	/*  75 */  pixels.Color(186,85,211),  /* medium_orchid */
	/*  75 */  pixels.Color(255,105,180),  /* hot_pink */
	/*  80 */  pixels.Color(220,20,60),  /* crimson */
	/*  85 */  pixels.Color(255,0,0),  /* red */
	/*  85 */  pixels.Color(255,69,0),  /* orange_red */
	/*  85 */  pixels.Color(255,140,0),  /* dark_orange */
	/*  85 */  pixels.Color(255,165,0),  /* orange */
	/*  85 */  pixels.Color(255,215,0),  /* gold */
	/*  85 */  pixels.Color(255,255,0),  /* yellow */
	/*  85 */  pixels.Color(0,128,0),  /* green */
	/*  85 */  pixels.Color(0,255,255),  /* aqua */
	/*  85 */  pixels.Color(0,255,255),  /* cyan */
	/*  85 */  pixels.Color(0,191,255),  /* deep_sky_blue */
	/*  85 */  pixels.Color(0,0,255),  /* blue */
	/*  85 */  pixels.Color(153,50,204),  /* dark_orchid */
	/*  85 */  pixels.Color(128,0,128),  /* purple */
	/*  92 */  pixels.Color(138,43,226),  /* blue_violet */
	/*  92 */  pixels.Color(139,0,139),  /* dark_magenta */
	/*  96 */  pixels.Color(173,255,47),  /* green_yellow */
	/*  96 */  pixels.Color(199,21,133),  /* medium_violet_red */
	/* 103 */  pixels.Color(50,205,50),  /* lime_green */
	/* 115 */  pixels.Color(0,250,154),  /* medium_spring_green */
	/* 119 */  pixels.Color(148,0,211),  /* dark_violet */
	/* 120 */  pixels.Color(255,20,147),  /* deep_pink */
	/* 126 */  pixels.Color(124,252,0),  /* lawn_green */
	/* 127 */  pixels.Color(127,255,0),  /* chartreuse */
	/* 127 */  pixels.Color(0,255,127),  /* spring_green */
	/* 170 */  pixels.Color(0,255,0),  /* lime */
	/* 170 */  pixels.Color(255,0,255),  /* magenta_/_fuchsia */
};

// Number of colors defined
int pixelColorsCount = *(&pixelColors + 1) - pixelColors;

// Number of NeoPixels attached to the ucontroller
#define NUM_PIXELS 72
#define MIN_LUMINOSITY 4
#define DIMMER 0.25  // Dim colors to this fraction; eventually attach this to a rotary encoder or potentiometer on an analog lead
#define COLOR_DISTANCE_REQ 0.33333  // New color bars must have color that is at least this fraction of Euclidean distance away from any other color currently in use
#define MAX_COLOR_DISTANCE sqrt(pow(256,2) + pow(256,2) + pow(256,2))  // Maximum possible euclidean distance between brightest and darkest colors
#define MIN_BARS 3
#define MAX_BARS 8
#define MIN_BAR_LENGTH 4
#define MAX_BAR_LENGTH 16
#define MIN_SPEED_PIXSEC 0.25
#define MAX_SPEED_PIXSEC 1.5
#define MIN_TRANSPARENCY 0.0
#define MAX_TRANSPARENCY 1.0
#define MIN_LOOPS_LIFESPAN 1
#define MAX_LOOPS_LIFESPAN 5

struct ColorBar {
  uint32_t length;
  uint32_t startPos;
  double curPos;
  PixelColor color;
  double speed;
  bool direction;
  double alpha;
  double taper;
  uint32_t lifeSpan;
} colorBars[MAX_BARS];


// Difference between two colors
double pointDistance(PixelColor c1, PixelColor c2, PixelColor mask, int base) {
  int c1m = (c1 & mask) >> base;
  int c2m = (c2 & mask) >> base;
  int cdiff = abs(c1m - c2m);
  double result = pow(cdiff, 2.0);

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_POINTDIST)
  char buf[100], resultf[40];  // c1mf[40], c2mf[40], cdiffsf[40], 
  dtostrf(result, 10, 3, &resultf[0]);
  sprintf(buf, "c1m: %d, c2m: %d, cdiff: %d, result: %s", c1m, c2m, cdiff, resultf);
  Serial.println(buf);
#endif

  return result;
}

// Euclidean distance between two colors in 3-space
int colorDistance(PixelColor c1, PixelColor c2) {
  double red =   /* red */   pointDistance(c1, c2, 0x00FF0000, 16);
  double green = /* green */ pointDistance(c1, c2, 0x0000FF00, 8);
  double blue = /* blue */  pointDistance(c1, c2, 0x000000FF, 0);
  double result = sqrt(red + green + blue);

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_COLORDIST)
  char buf[100], fred[40], fgreen[40], fblue[40], fresult[40];
  dtostrf(red, 10, 3, &fred[0]);
  dtostrf(green, 10, 3, &fgreen[0]);
  dtostrf(blue, 10, 3, &fblue[0]);
  dtostrf(result, 10, 3, &fresult[0]);
  sprintf(buf, "===== red: %s, green: %s, blue: %s, result: %s =====\n", fred, fgreen, fblue, fresult);
  Serial.println(buf);
#endif

  return result;
}

int barCount;

PixelColor getUnrelatedColor() {
  // Find a color in pixelColors table that is at least COLOR_DISTANCE_REQ away from any other color in use by a live colorbar
  int maxAttempts = 256, attempts = 0;
  PixelColor colorCandidate = 0, colorInUse = 0;
  int barNo = 0;
  int thisColDist = 0, farEnough = 0;

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
  char buf[100], pctMaxDist[15], colDistReq[15];
#endif

  while(attempts++ < maxAttempts) {
    int seed = analogRead(0);

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
    Serial.print("Seed: ");
    Serial.println(seed);
#endif

    randomSeed(seed);
    colorCandidate = pixelColors[random(pixelColorsCount)];
    int safeDistance = 0;
    for(barNo=0; barNo<barCount; barNo++) {
      colorInUse = colorBars[barNo].color;
      if(colorInUse > 0) {
        thisColDist = colorDistance(colorCandidate, colorInUse);
        farEnough = thisColDist / MAX_COLOR_DISTANCE >= COLOR_DISTANCE_REQ;
        if( farEnough ) {
          ++safeDistance;
        }
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
        dtostrf(thisColDist / MAX_COLOR_DISTANCE, 12, 4, &pctMaxDist[0]);
        dtostrf(COLOR_DISTANCE_REQ, 12, 4, &colDistReq[0]);
        sprintf(
          buf,
          "attempts: %d, barNo: %d (barCount: %d), colorCandidate: %lu, colorInUse: %lu, thisColDist: %d, farEnough: %d, thisColDist / MAX_COLOR_DISTANCE = %s >= COLOR_DISTANCE_REQ(%s)\n",
            attempts,     barNo,     barCount,      colorCandidate,      colorInUse,      thisColDist,     farEnough,     pctMaxDist, colDistReq);
        Serial.print(buf);
#endif
      }
    }
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_GETCOLOR)
    Serial.print("Attempts: ");
    Serial.println(attempts);
#endif
    if(safeDistance >= barCount) {
      break;
    }
  }
  return colorCandidate;
}

ColorBar newBar(int barNo) {
  ColorBar b;
  b.length = MIN_BAR_LENGTH + random(MAX_BAR_LENGTH - MIN_BAR_LENGTH);
  b.startPos = random(NUM_PIXELS);
  b.curPos = b.startPos;
  b.color = getUnrelatedColor();
  b.speed = MIN_SPEED_PIXSEC + (random(256) / 256.0) * (MAX_SPEED_PIXSEC - MIN_SPEED_PIXSEC);
  b.direction = random(2) >= 1 ? true : false;
  b.alpha = random(256) / 256.0;
  b.taper = 0.0;
  b.lifeSpan = MIN_LOOPS_LIFESPAN + random(MAX_LOOPS_LIFESPAN - MIN_LOOPS_LIFESPAN);

#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_NEWBAR)
  char buf[100];
  char fcur[20], fspeed[20];
  dtostrf(b.curPos, 12, 4, fcur);
  dtostrf(b.speed, 12, 4, fspeed);
  sprintf(buf, "Bar#%d: length=%lu, start=%lu, cur=%s, color=%8lx, speed=%s, dir=%d, life=%lu\n",  // , alpha=%s, taper=%s
    barNo,
    b.length,
    b.startPos,
    fcur,
    b.color,
    fspeed,
    b.direction,
    b.lifeSpan
  );
  Serial.print(buf);
#endif

  return b;
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  randomSeed(analogRead(0));

  barCount = MIN_BARS + random(MAX_BARS - MIN_BARS);
  // delay(1000);
  Serial.begin(115200);

  // Adafruit Trinket 5V 16 MHz.
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

// #if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_SETUP)
  char buf[100];
  sprintf(buf, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", barCount, 
    SERIAL_DEBUG & DEBUG_SETUP, 
    SERIAL_DEBUG & DEBUG_NEWBAR,
    SERIAL_DEBUG & DEBUG_GETCOLOR,
    SERIAL_DEBUG & DEBUG_COLORSMERGED,
    SERIAL_DEBUG & DEBUG_POSCHANGED,
    SERIAL_DEBUG & DEBUG_POINTDIST,
    SERIAL_DEBUG & DEBUG_COLORDIST
  );
  sprintf(buf, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", SERIAL_DEBUG, 
    DEBUG_SETUP, 
    DEBUG_NEWBAR,
    DEBUG_GETCOLOR,
    DEBUG_COLORSMERGED,
    DEBUG_POSCHANGED,
    DEBUG_POINTDIST,
    DEBUG_COLORDIST
  );
  Serial.print(buf);
// #endif

  for(int barNo=0; barNo < barCount; barNo++) {
    colorBars[barNo] = newBar(barNo);
  }
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
}


void loop() {
  pixels.clear();

  for (int barNo = 0; barNo < barCount; barNo++) {
    ColorBar* bar = &colorBars[barNo];
    for (uint32_t barPix = bar->curPos; barPix < bar->curPos + bar->length; barPix++) {
      uint32_t curPixColor = pixels.getPixelColor(barPix % NUM_PIXELS);
      unsigned int red, green, blue;
      unsigned long newColor;
      pixels.setPixelColor(
        barPix % NUM_PIXELS,
        newColor = pixels.Color(
          red =   ((((bar->color & (uint32_t)0x00FF0000) >> 16) * bar->alpha) + (((curPixColor & (uint32_t)0x00FF0000) >> 16) * (1.0 - bar->alpha))) / 2 * DIMMER,
          green = ((((bar->color & (uint32_t)0x0000FF00) >>  8) * bar->alpha) + (((curPixColor & (uint32_t)0x0000FF00) >>  8) * (1.0 - bar->alpha))) / 2 * DIMMER,
          blue =   (((bar->color & (uint32_t)0x000000FF)        * bar->alpha) + (curPixColor & (uint32_t)0x000000FF)          * (1.0 - bar->alpha))  / 2 * DIMMER
        )

      );


#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_COLORSMERGED)
      char buf[100];
      sprintf(buf, "bar#%d \tpix#%lu \tpix#%lu \tred:%X \tgreen:%X \tblue:%X \tcolor:%lX\n",
        barNo,
        barPix,
        (uint32_t)(barPix % NUM_PIXELS),
        red,
        green,
        blue,
        newColor
      );
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
    if(bar->curPos > NUM_PIXELS) {
      bar->curPos = 0;
    }
    if(bar->curPos < 0) {
      bar->curPos = NUM_PIXELS;
    }

    // bar->curPos = bar->curPos + (bar->direction ? 1 : -1);
#if defined(SERIAL_DEBUG) && (SERIAL_DEBUG & DEBUG_POSCHANGED)
    Serial.print("curPos: ");
    Serial.print(bar->curPos);
    Serial.print("\n");
#endif
  }

  pixels.show();  // Send the updated pixel colors to the hardware.
}
