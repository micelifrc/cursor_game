//
// Created by mich on 11/11/18.
//

#include <fstream>
#include "Scenario.h"

bool DynamicCharacter::try_move(Coord cd, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                const DynamicCharacter &pc) {
   if (can_move(cd, bg, dc, pc)) {
      move(cd);
      return true;
   } else {
      return false;
   }
}

bool DynamicCharacter::try_move(Coord cp, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                const DynamicCharacter &pc, std::vector<Coord> &update_pos) {
   if (try_move(cp, bg, dc, pc)) {
      update_pos.emplace_back(position);
      update_pos.emplace_back(position - cp);
      return true;
   } else {
      return false;
   }
}

bool DynamicCharacter::can_move(Coord cd, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                const DynamicCharacter &pc) {
   if (cd.x == 0 and cd.y == 0) return true;
   if (cd.x != 0 and cd.y != 0) return false;
   Coord dcd(0, 0);
   if (cd.x == 0) {
      dcd.y = cd.y > 0 ? 1 : -1;
   } else {
      dcd.x = cd.x > 0 ? 1 : -1;
   }
   std::vector<Coord> trajectory;
   for (Coord move = Coord(0, 0); move != cd; move += dcd) {
      trajectory.emplace_back(position + move + dcd);
   }
   for (const auto &pos : trajectory) {
      if (pos.x < 0 or pos.y < 0 or pos.x >= length or pos.y >= height) return false;
      if (bg.is_solid(pos)) return false;
      for (const auto &dchar: dc) {
         if (pos == dchar.position) return false;
      }
      if (pos == pc.position) return false;
   }
   return true;
}

const charPoint Scenario::playing_character('@', 1, true, true);

Scenario::Scenario(const std::unordered_map<char, charPoint> &images_map, std::string const &name_background_file,
                   int x, int y) : dynamic_pc(&playing_character, x, y) {
   std::ifstream input_file;
   input_file.open(name_background_file);
   if (not input_file) {
      throw std::logic_error("Input file " + name_background_file + " cannot be opened");
   }
   std::string file_string;
   char ch;
   while (input_file.get(ch)) {
      if (ch != '\n') {  // useful to write the map in a more meaningful way
         file_string.push_back(ch);
      }
   }
   input_file.close();
   background = Background(images_map, file_string);
}

// should also add a clock and move npcs
void Scenario::main(WINDOW *window) {
   keypad(window, true);
   initial_print(window);
   std::vector<Coord> update_pos;
   const char bell = '\a';
   while (true) {
      int command = update_print(window, update_pos);
      update_pos.clear();
      switch (command) {
         case KEY_UP: {
            dynamic_pc.try_move(Compass::north, background, dynamic_npcs, dynamic_pc, update_pos);
            break;
         }
         case KEY_RIGHT: {
            dynamic_pc.try_move(Compass::east, background, dynamic_npcs, dynamic_pc, update_pos);
            break;
         }
         case KEY_DOWN: {
            dynamic_pc.try_move(Compass::south, background, dynamic_npcs, dynamic_pc, update_pos);
            break;
         }
         case KEY_LEFT: {
            dynamic_pc.try_move(Compass::west, background, dynamic_npcs, dynamic_pc, update_pos);
            break;
         }
         case KEY_CLOSE :
         case 27: {
            return;
         }
      }
   }
}

void Scenario::initial_print(WINDOW *window) const {
   wclear(window);
   background.print(window);
   for (auto const &npc: dynamic_npcs) {
      npc.print(window);
   }
   dynamic_pc.print(window);
   curs_set(0);
   noecho();
   cbreak();
   wrefresh(window);
}

int Scenario::update_print(WINDOW *window, std::vector<Coord> const &update_coord) const {
   for (const auto &cd: update_coord) {
      background.print(window, cd);
   }
   for (auto const &npc: dynamic_npcs) {
      npc.print(window);
   }
   dynamic_pc.print(window);
   curs_set(0);
   noecho();
   cbreak();
   wrefresh(window);
   return wgetch(window);
}
