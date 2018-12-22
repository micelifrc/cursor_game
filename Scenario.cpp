//
// Created by mich on 11/11/18.
//

#include <fstream>
#include <random>
#include <limits>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "Scenario.h"

MarsigliaXorshift::MarsigliaXorshift(){
   std::random_device rd;
   std::mt19937_64 eng(rd());
   std::uniform_int_distribution<unsigned long> distr;
   coefficients[0] = distr(eng);
   coefficients[1] = distr(eng);
   coefficients[2] = distr(eng);
}

unsigned MarsigliaXorshift::rand(unsigned limit){
   coefficients[0] ^= coefficients[0] << 16;
   coefficients[0] ^= coefficients[0] >> 5;
   unsigned long temp = coefficients[0] << 1;
   coefficients[0] = coefficients[1];
   coefficients[1] = coefficients[2];
   coefficients[2] = temp ^ coefficients[0] ^ coefficients[1];
   return static_cast<unsigned>(coefficients[2] % limit);
}

void DynamicCharacter::update_character() {
   switch (orientation) {
      case Compass::north: {
         character.output_char = '^';
         break;
      }
      case Compass::east: {
         character.output_char = '>';
         break;
      }
      case Compass::south: {
         character.output_char = 'v';
         break;
      }
      case Compass::west: {
         character.output_char = '<';
         break;
      }
   }
}

bool DynamicCharacter::try_move_pc(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                   const DynamicCharacter &pc) {
   if (can_move(cp, bg, dc, pc)) {
      move(cp);
      return true;
   } else {
      rotate(cp);
      return false;
   }
}

bool DynamicCharacter::try_move_npc(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                    const DynamicCharacter &pc, std::vector<Coord> &update_pos) {
   if (can_move(cp, bg, dc, pc)) {
      update_pos.push_back(position);
      move(cp);
      return true;
   } else {
      rotate(cp);
      return false;
   }
}

bool DynamicCharacter::can_move(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc,
                                const DynamicCharacter &pc) {
   if (not(Coord(orientation) + Coord(cp)).is_zero()) {
      Coord new_position = position + Coord(cp);
      if (new_position.x < 0 or new_position.x >= length or new_position.y < 0 or new_position.y >= height) {
         return false;
      }
      if (bg.get_point(new_position).is_solid) {
         return false;
      }
      if (new_position == pc.position) {
         return false;
      }
      for (const auto &npc: dc) {
         if (new_position == npc.position) {
            return false;
         }
      }
   }
   return true;
}

void DynamicCharacter::rotate(Compass cp) {
   orientation = cp;
   update_character();
}

void DynamicCharacter::move(Compass cp) {
   if (not(Coord(cp) + Coord(orientation)).is_zero()) {
      position += Coord(cp);
   }
   rotate(cp);
}

const charPoint Scenario::playing_character('v', 1, true, true);

Scenario::Scenario(const std::unordered_map<char, charPoint> &images_map, std::string const &name_background_file,
                   int x, int y) : dynamic_pc(x, y, Compass::south, playing_character.is_bold,
                                              playing_character.is_solid, playing_character.color_code) {
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

void Scenario::main(WINDOW *window) {
   keypad(window, true);
   initial_print(window);
   std::vector<Coord> update_pos;
   bool keep_moving = true;
   std::thread move_npcs_thread(&Scenario::move_npcs, this, std::ref(window), std::ref(update_pos),
                                std::ref(keep_moving));
   move_pc(window);
   keep_moving = false;
   move_npcs_thread.join();
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

void Scenario::update_print(WINDOW *window, std::vector<Coord> &update_coord) const {
   for (const auto &cd: update_coord) {
      background.print(window, cd);
   }
   update_coord.clear();
   for (auto const &npc: dynamic_npcs) {
      npc.print(window);
   }
   wrefresh(window);
}

bool Scenario::is_free_position(Coord cd) const {
   if (cd.x < 0 or cd.x >= length or cd.y < 0 or cd.y >= height) {
      return false;
   }
   if (background.get_point(cd).is_solid) {
      return false;
   }
   if (cd == dynamic_pc.position) {
      return false;
   }
   for (const auto &npc: dynamic_npcs) {
      if (cd == npc.position) {
         return false;
      }
   }
   return true;
}

void Scenario::add_random_npcs(unsigned number) {
   while (number != 0) {
      bool is_free_space = false;
      for (unsigned idx = 0; idx != length * height; ++idx) {
         if (is_free_position(Coord(idx % length, idx / length))) {
            is_free_space = true;
            break;
         }
      }
      if (not is_free_space) {
         throw std::logic_error("No free space for call of Scenario::add_random_npcs(unsigned)");
      }
      while (true) {
         unsigned candidate = rand(length * height * 4);
         Coord position(candidate % length, (candidate / length) % height);
         if (is_free_position(position)) {
            add_npc(position, Compass(candidate / (length * height)));
            --number;
            break;
         }
      }
   }
}

void Scenario::move_pc(WINDOW *window) {
   while (true) {
      Coord old_pos = dynamic_pc.position;
      int command = wgetch(window);
      switch (command) {
         case KEY_UP: {
            dynamic_pc.try_move_pc(Compass::north, background, dynamic_npcs, dynamic_pc);
            background.print(window, old_pos);
            dynamic_pc.print(window);
            break;
         }
         case KEY_RIGHT: {
            dynamic_pc.try_move_pc(Compass::east, background, dynamic_npcs, dynamic_pc);
            background.print(window, old_pos);
            dynamic_pc.print(window);
            break;
         }
         case KEY_DOWN: {
            dynamic_pc.try_move_pc(Compass::south, background, dynamic_npcs, dynamic_pc);
            background.print(window, old_pos);
            dynamic_pc.print(window);
            break;
         }
         case KEY_LEFT: {
            dynamic_pc.try_move_pc(Compass::west, background, dynamic_npcs, dynamic_pc);
            background.print(window, old_pos);
            dynamic_pc.print(window);
            break;
         }
         case KEY_CLOSE :
         case 'q': {
            return;
         }
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(35));
      while(kbhit()){
         wgetch(window);
      }
   }
}

void Scenario::move_npcs(WINDOW *window, std::vector<Coord> &update_pos, bool &keep_moving) {
   using namespace std::chrono_literals;
   while (keep_moving) {
      for (auto &npc : dynamic_npcs) {
         unsigned randomic = rand(12);
         if (randomic < 4) {
            npc.try_move_npc(Compass(randomic), background, dynamic_npcs, dynamic_pc, update_pos);
         } else if (randomic < 6) {
            npc.try_step_npc(background, dynamic_npcs, dynamic_pc, update_pos);
         }
      }
      update_print(window, update_pos);
      std::this_thread::sleep_for(std::chrono::milliseconds(210));
   }
}

bool Scenario::kbhit()
{
   termios term;
   tcgetattr(0, &term);

   termios term2 = term;
   term2.c_lflag &= ~ICANON;
   tcsetattr(0, TCSANOW, &term2);

   int byteswaiting;
   ioctl(0, FIONREAD, &byteswaiting);

   tcsetattr(0, TCSANOW, &term);

   return byteswaiting > 0;
}