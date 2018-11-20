#include <iostream>
#include <string>
#include <unordered_map>
#include "Scenario.h"

// this should be a member function called in the constructor of the game. The game will contain a Screen, together with several other objects (like
void initialize_character_map(std::unordered_map<char, charPoint> &map) {
#ifndef INITIALIZE_CHARACTER_MAP
#define INITIALIZE_CHARACTER_MAP
   start_color();
   init_pair(1, COLOR_YELLOW, COLOR_BLUE);  // protagonist This should be called only in the constructor of the game
   init_pair(2, COLOR_WHITE, COLOR_BLACK);  // character
   map['@'] = charPoint('@', 2, true);
   init_pair(3, COLOR_BLACK, COLOR_BLACK);  // asphalt
   map['a'] = charPoint(' ', 3);
   init_pair(4, COLOR_YELLOW, COLOR_GREEN);  // grass
   map['g'] = charPoint('^', 4);
   init_pair(5, COLOR_BLACK, COLOR_GREEN);  // tree
   map['Y'] = charPoint('Y', 5, true);
   init_pair(6, COLOR_WHITE, COLOR_BLACK);  // mountain
   map['^'] = charPoint('^', 6, true);
   init_pair(7, COLOR_BLUE, COLOR_CYAN);  // water
   map['w'] = charPoint('~', 7, true);
   init_pair(8, COLOR_RED, COLOR_YELLOW);  // desert
   map['d'] = charPoint('~', 8);
#endif
}

int main() {
   initscr();
   std::unordered_map<char, charPoint> images_map;
   initialize_character_map(images_map);
   Scenario scenario(images_map, "backgrounds/geo_map.txt", 0, 0);
   charPoint stdnpc('@', 2, true);
   scenario.add_npc(&stdnpc, 103, 23);
   scenario.main(stdscr);
   endwin();
   return 0;
}