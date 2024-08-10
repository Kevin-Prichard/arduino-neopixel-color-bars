
#include <Adafruit_NeoPixel.h>


typedef uint32_t PixelColor;

// Number of NeoPixels attached to the ucontroller
#define NUM_PIXELS 144

// Arduino pin used to drive the NeoPixel array
#define PIN 6

// Instantiate Adafruit NeoPixel handler
extern Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

   /*
   Color palette - ripped from Netscape's original HTML color set,
                   and limited for contrast and colorfulness

   Sorted in contrast order, by the mean of absolute diffs between r, g and b
   Lower contrast colors removed, see colors_by_contrast.txt for complete set

   contrast value                R     G     B        hex val =  color name
   ==============               ===   ===   ===       ====================== */
extern const PixelColor pixelColors[] = {
    /*   85 */    pixels.Color( 128,    0,    0),  /* 800000  =  maroon */
    /*   93 */    pixels.Color( 139,    0,    0),  /* 8B0000  =  dark red */
    /*   82 */    pixels.Color( 165,   42,   42),  /* A52A2A  =  brown */
    /*   96 */    pixels.Color( 178,   34,   34),  /* B22222  =  firebrick */
    /*  133 */    pixels.Color( 220,   20,   60),  /* DC143C  =  crimson */
    /*  170 */    pixels.Color( 255,    0,    0),  /* FF0000  =  red */
    /*  123 */    pixels.Color( 255,   99,   71),  /* FF6347  =  tomato */
    /*  117 */    pixels.Color( 255,  127,   80),  /* FF7F50  =  coral */
    /*   75 */    pixels.Color( 205,   92,   92),  /* CD5C5C  =  indian red */
    /*   75 */    pixels.Color( 240,  128,  128),  /* F08080  =  light coral */
    /*   74 */    pixels.Color( 233,  150,  122),  /* E9967A  =  dark salmon */
    /*   91 */    pixels.Color( 250,  128,  114),  /* FA8072  =  salmon */
    /*   89 */    pixels.Color( 255,  160,  122),  /* FFA07A  =  light salmon */
    /*  170 */    pixels.Color( 255,   69,    0),  /* FF4500  =  orange red */
    /*  170 */    pixels.Color( 255,  140,    0),  /* FF8C00  =  dark orange */
    /*  170 */    pixels.Color( 255,  165,    0),  /* FFA500  =  orange */
    /*  170 */    pixels.Color( 255,  215,    0),  /* FFD700  =  gold */
    /*  115 */    pixels.Color( 184,  134,   11),  /* B8860B  =  dark golden rod */
    /*  124 */    pixels.Color( 218,  165,   32),  /* DAA520  =  golden rod */
    /*   45 */    pixels.Color( 238,  232,  170),  /* EEE8AA  =  pale golden rod */
    /*   55 */    pixels.Color( 189,  183,  107),  /* BDB76B  =  dark khaki */
    /*   67 */    pixels.Color( 240,  230,  140),  /* F0E68C  =  khaki */
    /*   85 */    pixels.Color( 128,  128,    0),  /* 808000  =  olive */
    /*  170 */    pixels.Color( 255,  255,    0),  /* FFFF00  =  yellow */
    /*  103 */    pixels.Color( 154,  205,   50),  /* 9ACD32  =  yellow green */
    /*   71 */    pixels.Color( 107,  142,   35),  /* 6B8E23  =  olive drab */
    /*  168 */    pixels.Color( 124,  252,    0),  /* 7CFC00  =  lawn green */
    /*  170 */    pixels.Color( 127,  255,    0),  /* 7FFF00  =  chartreuse */
    /*  139 */    pixels.Color( 173,  255,   47),  /* ADFF2F  =  green yellow */
    /*   67 */    pixels.Color(   0,  100,    0),  /* 006400  =  dark green */
    /*   85 */    pixels.Color(   0,  128,    0),  /* 008000  =  green */
    /*   70 */    pixels.Color(  34,  139,   34),  /* 228B22  =  forest green */
    /*  170 */    pixels.Color(   0,  255,    0),  /* 00FF00  =  lime */
    /*  103 */    pixels.Color(  50,  205,   50),  /* 32CD32  =  lime green */
    /*   63 */    pixels.Color( 144,  238,  144),  /* 90EE90  =  light green */
    /*   66 */    pixels.Color( 152,  251,  152),  /* 98FB98  =  pale green */
    /*  167 */    pixels.Color(   0,  250,  154),  /* 00FA9A  =  medium spring green */
    /*  170 */    pixels.Color(   0,  255,  127),  /* 00FF7F  =  spring green */
    /*   62 */    pixels.Color(  46,  139,   87),  /* 2E8B57  =  sea green */
    /*   69 */    pixels.Color( 102,  205,  170),  /* 66CDAA  =  medium aqua marine */
    /*   79 */    pixels.Color(  60,  179,  113),  /* 3CB371  =  medium sea green */
    /*   97 */    pixels.Color(  32,  178,  170),  /* 20B2AA  =  light sea green */
    /*   85 */    pixels.Color(   0,  128,  128),  /* 008080  =  teal */
    /*   93 */    pixels.Color(   0,  139,  139),  /* 008B8B  =  dark cyan */
    /*  170 */    pixels.Color(   0,  255,  255),  /* 00FFFF  =  aqua */
    /*  170 */    pixels.Color(   0,  255,  255),  /* 00FFFF  =  cyan */
    /*  139 */    pixels.Color(   0,  206,  209),  /* 00CED1  =  dark turquoise */
    /*  107 */    pixels.Color(  64,  224,  208),  /* 40E0D0  =  turquoise */
    /*   91 */    pixels.Color(  72,  209,  204),  /* 48D1CC  =  medium turquoise */
    /*   85 */    pixels.Color( 127,  255,  212),  /* 7FFFD4  =  aqua marine */
    /*   73 */    pixels.Color(  70,  130,  180),  /* 4682B4  =  steel blue */
    /*   91 */    pixels.Color( 100,  149,  237),  /* 6495ED  =  corn flower blue */
    /*  170 */    pixels.Color(   0,  191,  255),  /* 00BFFF  =  deep sky blue */
    /*  150 */    pixels.Color(  30,  144,  255),  /* 1E90FF  =  dodger blue */
    /*   67 */    pixels.Color( 135,  206,  235),  /* 87CEEB  =  sky blue */
    /*   77 */    pixels.Color( 135,  206,  250),  /* 87CEFA  =  light sky blue */
    /*   58 */    pixels.Color(  25,   25,  112),  /* 191970  =  midnight blue */
    /*   85 */    pixels.Color(   0,    0,  128),  /* 000080  =  navy */
    /*   93 */    pixels.Color(   0,    0,  139),  /* 00008B  =  dark blue */
    /*  137 */    pixels.Color(   0,    0,  205),  /* 0000CD  =  medium blue */
    /*  170 */    pixels.Color(   0,    0,  255),  /* 0000FF  =  blue */
    /*  107 */    pixels.Color(  65,  105,  225),  /* 4169E1  =  royal blue */
    /*  122 */    pixels.Color( 138,   43,  226),  /* 8A2BE2  =  blue violet */
    /*   87 */    pixels.Color(  75,    0,  130),  /* 4B0082  =  indigo */
    /*   52 */    pixels.Color(  72,   61,  139),  /* 483D8B  =  dark slate blue */
    /*   77 */    pixels.Color( 106,   90,  205),  /* 6A5ACD  =  slate blue */
    /*   89 */    pixels.Color( 123,  104,  238),  /* 7B68EE  =  medium slate blue */
    /*   71 */    pixels.Color( 147,  112,  219),  /* 9370DB  =  medium purple */
    /*   93 */    pixels.Color( 139,    0,  139),  /* 8B008B  =  dark magenta */
    /*  141 */    pixels.Color( 148,    0,  211),  /* 9400D3  =  dark violet */
    /*  103 */    pixels.Color( 153,   50,  204),  /* 9932CC  =  dark orchid */
    /*   84 */    pixels.Color( 186,   85,  211),  /* BA55D3  =  medium orchid */
    /*   85 */    pixels.Color( 128,    0,  128),  /* 800080  =  purple */
    /*   72 */    pixels.Color( 238,  130,  238),  /* EE82EE  =  violet */
    /*  170 */    pixels.Color( 255,    0,  255),  /* FF00FF  =  magenta / fuchsia */
    /*   71 */    pixels.Color( 218,  112,  214),  /* DA70D6  =  orchid */
    /*  119 */    pixels.Color( 199,   21,  133),  /* C71585  =  medium violet red */
    /*   71 */    pixels.Color( 219,  112,  147),  /* DB7093  =  pale violet red */
    /*  157 */    pixels.Color( 255,   20,  147),  /* FF1493  =  deep pink */
    /*  100 */    pixels.Color( 255,  105,  180),  /* FF69B4  =  hot pink */
    /*   49 */    pixels.Color( 255,  182,  193),  /* FFB6C1  =  light pink */
    /*   80 */    pixels.Color( 139,   69,   19),  /* 8B4513  =  saddle brown */
    /*   77 */    pixels.Color( 160,   82,   45),  /* A0522D  =  sienna */
    /*  120 */    pixels.Color( 210,  105,   30),  /* D2691E  =  chocolate */
    /*   95 */    pixels.Color( 205,  133,   63),  /* CD853F  =  peru */
    /*   99 */    pixels.Color( 244,  164,   96),  /* F4A460  =  sandy brown */
    /*   58 */    pixels.Color( 222,  184,  135),  /* DEB887  =  burly wood */
    /*   47 */    pixels.Color( 210,  180,  140),  /* D2B48C  =  tan */
    /*   49 */    pixels.Color( 255,  228,  181),  /* FFE4B5  =  moccasin */
    /*   55 */    pixels.Color( 255,  222,  173),  /* FFDEAD  =  navajo white */
    /*   47 */    pixels.Color( 255,  218,  185),  /* FFDAB9  =  peach puff */

};

// Determine the number of colors actually defined
extern int pixelColorsCount = *(&pixelColors + 1) - pixelColors;

extern const unsigned char redIndex[] = {
    30,
    31,
    33,
    38,
    39,
    45,
    46,
    47,
    48,
    50,
    59,
    65,
    66,
    67,
    68,
    64,
    60,
    43,
    32,
    40,
    34,
    42,
    51,
    69,
    57,
    52,
    72,
    71,
    58,
    41,
    73,
    26,
    74,
    27,
    28,
    54,
    0,
    22,
    80,
    62,
    63,
    70,
    1,
    76,
    101,
    35,
    75,
    77,
    36,
    78,
    24,
    102,
    2,
    29,
    3,
    17,
    79,
    20,
    86,
    8,
    104,
    103,
    107,
    18,
    85,
    87,
    4,
    106,
    10,
    19,
    83,
    9,
    21,
    105,
    11,
    5,
    6,
    7,
    12,
    13,
    14,
    15,
    16,
    23,
    84,
    88,
    89,
    90,
    109,
    110,
    111,
};

extern const unsigned char greenIndex[] = {
    0,
    1,
    5,
    65,
    66,
    67,
    68,
    71,
    76,
    77,
    80,
    84,
    4,
    88,
    86,
    64,
    3,
    2,
    70,
    78,
    72,
    13,
    101,
    102,
    79,
    73,
    8,
    6,
    30,
    74,
    69,
    89,
    103,
    75,
    85,
    87,
    7,
    9,
    11,
    22,
    31,
    45,
    57,
    83,
    104,
    17,
    32,
    40,
    46,
    14,
    26,
    60,
    58,
    10,
    12,
    105,
    15,
    18,
    43,
    42,
    107,
    90,
    20,
    106,
    59,
    24,
    34,
    41,
    50,
    62,
    63,
    52,
    16,
    111,
    110,
    51,
    109,
    21,
    19,
    35,
    38,
    36,
    27,
    23,
    28,
    29,
    33,
    39,
    47,
    48,
    54,
};

extern const unsigned char blueIndex[] = {
    0,
    1,
    5,
    13,
    14,
    15,
    16,
    22,
    23,
    27,
    28,
    30,
    31,
    33,
    17,
    101,
    103,
    18,
    3,
    32,
    26,
    2,
    102,
    29,
    24,
    34,
    4,
    104,
    6,
    7,
    40,
    8,
    105,
    20,
    64,
    42,
    11,
    10,
    12,
    39,
    9,
    45,
    65,
    80,
    71,
    86,
    106,
    46,
    66,
    72,
    76,
    21,
    107,
    35,
    87,
    88,
    36,
    38,
    19,
    41,
    43,
    110,
    57,
    89,
    109,
    111,
    90,
    52,
    78,
    67,
    73,
    51,
    50,
    77,
    79,
    54,
    85,
    75,
    69,
    70,
    62,
    58,
    74,
    83,
    63,
    47,
    48,
    59,
    60,
    68,
    84,
};

