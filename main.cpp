#include <iostream>
#include <string>
#include "Screen.h"

// this should be a member function called in the constructor of the game. The game will contain a Screen, together with several other objects (like
void initialize_color_pairs() {
   start_color();
   init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // protagonist
   init_pair(2, COLOR_WHITE, COLOR_BLACK);  // character
   init_pair(3, COLOR_YELLOW, COLOR_GREEN);  // grass
   init_pair(4, COLOR_BLUE, COLOR_CYAN);  // water
   init_pair(5, COLOR_WHITE, COLOR_BLACK);  // mountain
   init_pair(6, COLOR_RED, COLOR_YELLOW);  // desert
}

int main() {
   initscr();
   initialize_color_pairs();
   std::string grass(length * height, 'g');
   Screen<charPoint> screen(grass);
   print(stdscr, screen);
   endwin();
   return 0;
}