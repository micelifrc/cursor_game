//
// Created by mich on 10/11/18.
//

#include "Screen.h"

Coord::Coord(const Compass cps) {
   switch (cps) {
      case Compass::north : {
         x = 0;
         y = -1;
         return;
      }
      case Compass::east : {
         x = 1;
         y = 0;
         return;
      }
      case Compass::south : {
         x = 0;
         y = 1;
         return;
      }
      case Compass::west : {
         x = -1;
         y = 0;
         return;
      }
   }
}

charPoint::charPoint(char ch) : is_solid{false}, is_bold{false} {
   switch (ch) {
      case '@': {  // character
#ifndef CHARACTER_COLOR
#define CHARACTER_COLOR 2
         init_pair(CHARACTER_COLOR, COLOR_WHITE, COLOR_BLACK);  // character
#endif
         output_char = '@';
         color_code = CHARACTER_COLOR;
         is_solid = true;
         return;
      }
      case 'a': {  // asphalt
#ifndef ASPHALT_COLOR
         #define ASPHALT_COLOR 3
         init_pair(ASPHALT_BLACK, COLOR_WHITE, COLOR_BLACK);  // character
#endif
         output_char = ' ';
         color_code = ASPHALT_COLOR;
         return;
      }
      case 'g': {  // grass
#ifndef GRASS_COLOR
#define GRASS_COLOR 3
         output_char = '^';

         color_code = GRASS_COLOR;
         return;
      }
      case 'y': {  // tree
         output_char = 'Y';
         color_code = 4;
         return;
      }
      case 'm': {  // mountain
         output_char = '^';
         color_code = 5;
         is_solid = true;
         return;
      }
      case 'w': {  // water
         output_char = '~';
         color_code =;
         is_solid = true;
         return;
      }
      case 'd': {  // desert
         output_char = '~';
         color_code = 6;
         return;
      }
      default: {
         throw std::logic_error("Invalid costructor of charPoint with input " + ch);
      }
   }
}

void print(WINDOW *window, const charPoint &pt, unsigned x, unsigned y) {
   wattron(window, COLOR_PAIR(pt.color_code));
   if (pt.is_bold) wattron(window, A_BOLD);
   mvwprintw(window, y, x, "%c", pt.output_char);
   if (pt.is_bold) wattroff(window, A_BOLD);
   wattroff(window, COLOR_PAIR(pt.color_code));
}

void print(WINDOW *window, const charPoint &pt, const Coord cd) {
   print(window, pt, cd.x, cd.y);
}