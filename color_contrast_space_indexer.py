#!/usr/bin/env python3

import json
import re, sys
from typing import List, Generator, Dict, Text, Tuple


def abs_mean_diff(rgb: List[int]) -> float:
    return (abs(rgb[0] - rgb[1]) +
            abs(rgb[1] - rgb[2]) +
            abs(rgb[2] - rgb[0])) / 3


COLOR_DEFS_RX = re.compile(
    r'^ *\t(?P<name>[^\t]+)'
    r'\t\#(?P<hex>[0-9A-F]{6})'
    r'\t\((?P<rgb>\d+,\d+,\d+)\)\s*$', re.IGNORECASE)


def get_color_defs(pathname: str) -> Generator[Dict, None, None]:  # [str, str, List[int]], None, None]:
    with open(pathname, 'r') as f:
        for line in f:
            match = COLOR_DEFS_RX.match(line)
            if not match:
                print(f"Skipping line: {line}")
                continue
            parts = match.groupdict()
            yield {
                "name": parts['name'],
                "hex": parts['hex'],
                "rgb": (rgb:=list(map(int, parts['rgb'].split(',')))),
                "mean_diff": abs_mean_diff(rgb),
            }


def get_color_indices(color_defs_pathname: Text,
                 color_index_pathname: Text,
                 contrast_threshold:float=45):

    def primary_index(colors: List[Tuple[int, Dict]],
                      primaryIndex: int) -> List[int]:
        return [idx for idx, color_def in
                sorted(colors, key=lambda x: x[1]['rgb'][primaryIndex])]

    with open(color_index_pathname, 'w') as f:
        colors = [(position, color_def) for position, color_def in
                  enumerate(get_color_defs(color_defs_pathname))
                  if color_def["mean_diff"] > contrast_threshold]

    indices = {
        "color_defs": colors,
        "red": primary_index(colors, 0),
        "green": primary_index(colors, 1),
        "blue": primary_index(colors, 2),
    }

    if not color_index_pathname:
        return indices

    with open(color_index_pathname, 'w') as f:
        f.write("""
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
""")
        for _, color_def in indices["color_defs"]:
            f.write(f"    /* {round(color_def['mean_diff'], 0): 4.0f} */"
                    f"    pixels.Color("
                    f"{', '.join([f'{val: 4d}' for val in color_def['rgb']])}"
                    f"),  /* {color_def['hex']}"
                    f"  =  {color_def['name']} */\n")
        f.write("""
};

// Determine the number of colors actually defined
extern int pixelColorsCount = *(&pixelColors + 1) - pixelColors;

""")

        for primary in ["red", "green", "blue"]:
            f.write(f"extern const unsigned char {primary}Index[] = {{\n")
            for idx in indices[primary]:
                f.write(f"    {idx},\n")
            f.write("};\n\n")

    return indices


def main():
    idx = get_color_indices(sys.argv[1], sys.argv[2])


if __name__ == '__main__':
    main()
