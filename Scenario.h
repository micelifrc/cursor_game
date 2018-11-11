//
// Created by mich on 11/11/18.
//

#ifndef RPGGAME_SCENARIO_H
#define RPGGAME_SCENARIO_H

#include <iostream>
#include <vector>
#include "Background.h"

struct DynamicCharacter {
   const charPoint *character;
   Coord position;
   Compass orientation;

   explicit DynamicCharacter(const charPoint *character_ = nullptr, Coord position_ = Coord(),
                             Compass orientation_ = Compass::south) : character{character_}, position{position_},
                                                                      orientation{orientation_} {}

   DynamicCharacter(const charPoint *character_, int x, int y, Compass orientation_ = Compass::south) : character{
         character_}, position(x, y), orientation{orientation_} {}

   inline void move(Compass cp) { move(Coord(cp)); }

   inline void move(Coord cd) {
      position += cd;
      orientation = make_compass_from_coord(cd);
   }

   inline bool try_move(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc,
                        const DynamicCharacter &pc) { return try_move(Coord(cp), bg, dc, pc); }

   bool try_move(Coord cd, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc);

   inline bool
   try_move(Compass cp, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc,
            std::vector<Coord> &update_pos) { return try_move(Coord(cp), bg, dc, pc, update_pos); }

   bool try_move(Coord cp, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc,
                 std::vector<Coord> &update_pos);

   inline void print(WINDOW *window = stdscr) const { character->print(window, position); }

private:
   bool can_move(Coord cd, const Background &bg, std::vector<DynamicCharacter> const &dc, const DynamicCharacter &pc);
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
                   Coord initial_position) : Scenario(images_map, name_background_file, initial_position.x,
                                                      initial_position.y) {}

   void main(WINDOW *window = stdscr);

private:
   void initial_print(WINDOW *window = stdscr) const;

   int update_print(WINDOW *window, std::vector<Coord> const &update_coord) const;
};


#endif //RPGGAME_SCENARIO_H
