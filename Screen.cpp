//
// Created by mich on 10/11/18.
//

#include "Screen.h"

Coord::Coord(Compass cps) {
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

charPoint::charPoint(char ch) : is_bold{false} {
   switch (ch) {
      case '@': {  // character
         output_char = '@';
         color_code = 2;
         is_solid = true;
         return;
      }
      case 'g': {  // grass
         output_char = '^';
         color_code = 3;
         is_solid = false;
         return;
      }
      case 'w': {  // water
         output_char = '~';
         color_code = 4;
         is_solid = true;
         return;
      }
      case 'm': {  // mountain
         output_char = '^';
         color_code = 5;
         is_solid = true;
         return;
      }
      case 'd': {  // desert
         output_char = '~';
         color_code = 6;
         is_solid = false;
         return;
      }
      default:{
         throw std::logic_error("Invalid costructor of charPoint with input "+ch);
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