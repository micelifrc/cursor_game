//
// Created by mich on 11/11/18.
//

#ifndef RPGGAME_SCENARIO_H
#define RPGGAME_SCENARIO_H

#include <iostream>
#include <vector>
#include "Background.h"

struct DynamicCharacter {
   charPoint character;
   Coord position;
   Compass orientation;

   explicit DynamicCharacter(Coord position_ = Coord(), Compass orientation_ = Compass::south, bool is_bold_ = false,
                             bool is_solid_ = true, unsigned color_code_ = 2) :
         character(' ', color_code_, is_solid_, is_bold_), position{position_},
         orientation{orientation_} { update_character(); }

   DynamicCharacter(int x, int y, Compass orientation_ = Compass::south, bool is_bold_ = false,
                    bool is_solid_ = true, unsigned color_code_ = 2) :
         DynamicCharacter(Coord(x, y), orientation_, is_bold_, is_solid_, color_code_) {}

   bool
   try_move_pc(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc);

   bool
   try_move_npc(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc,
                std::vector<Coord> &update_pos);

   inline bool try_step_pc(const Background &bg, std::vector<DynamicCharacter> const &dc,
                           const DynamicCharacter &pc) { return try_move_pc(orientation, bg, dc, pc); }

   inline bool try_step_npc(const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc,
                            std::vector<Coord> &update_pos) {
      return try_move_npc(orientation, bg, dc, pc, update_pos);
   }

   inline void print(WINDOW *window = stdscr) const { character.print(window, position); }

private:
   bool can_move(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc);

   void rotate(Compass cp);

   void move(Compass cp);

   void update_character();
};

class Scenario {
   DynamicCharacter dynamic_pc;
   std::vector<DynamicCharacter> dynamic_npcs;
   Background background;
   static const charPoint playing_character;
public:
   Scenario() = default;

   Scenario(const std::unordered_map<char, charPoint> &images_map, std::string const &name_background_file, int x = 0,
            int y = 0);

   inline Scenario(const std::unordered_map<char, charPoint> &images_map, std::string const &name_background_file,
                   Coord initial_position) :
         Scenario(images_map, name_background_file, initial_position.x, initial_position.y) {}

   template<typename... Args>
   void add_npc(Args... args) { dynamic_npcs.emplace_back(args...); };

   void add_random_npcs(unsigned number);

   void main(WINDOW *window = stdscr);

private:
   bool is_free_position(Coord cd) const;

   void initial_print(WINDOW *window = stdscr) const;

   void update_print(WINDOW *window, std::vector<Coord> &update_coord) const;

   void move_pc(WINDOW *window);

   void move_npcs(WINDOW *window, std::vector<Coord> &update_pos, bool &keep_moving);
};


#endif //RPGGAME_SCENARIO_H
