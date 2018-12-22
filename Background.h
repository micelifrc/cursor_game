//
// Created by mich on 10/11/18.
//

#ifndef RPGGAME_BACKGROUND_H
#define RPGGAME_BACKGROUND_H


// This is a structure to draw a full page in the terminal

#include <iostream>
#include <array>
#include <unordered_map>
#include "ncurses.h"

// screen size
#define height 55
#define length 205

enum struct Compass : unsigned {
   north = 0, east = 1, south = 2, west = 3
};

Compass make_compass(unsigned n);

struct Coord {
   int x, y;

   explicit Coord(int x_ = 0, int y_ = 0) : x{x_}, y{y_} {}

   explicit Coord(Compass cps);

   Coord& operator+=(const Coord& rhs);
   Coord& operator-=(const Coord& rhs);
   bool is_zero() const { return x == 0 and y == 0; }
};

inline Coord operator+(Coord lhs, const Coord& rhs) { return lhs += rhs; }
inline Coord operator-(Coord lhs, const Coord& rhs) { return lhs -= rhs; }
inline bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }
inline bool operator!=(const Coord& lhs, const Coord& rhs) { return not (lhs == rhs); }

Coord make_coord_from_compass(unsigned n);
Compass make_compass_from_coord(Coord const &cd);

struct charPoint {
   char output_char;
   unsigned color_code;
   bool is_solid;
   bool is_bold;

   explicit charPoint(char output_char_ = ' ', unsigned color_code_ = 0, bool is_solid_ = false,
                      bool is_blod_ = false) noexcept : output_char{output_char_}, color_code{color_code_},
                                                        is_solid{is_solid_}, is_bold{is_blod_} {}

   void print(WINDOW *window, int x, int y) const;
   inline void print(WINDOW *window, Coord cd = Coord()) const { print(window, cd.x, cd.y); }
};

struct Background {
   std::array<std::array<const charPoint *, length>, height> matrix;

   Background();

   explicit Background(const std::unordered_map<char, charPoint> &images_map);

   explicit Background(std::array<std::array<const charPoint *, length>, height> matrix_) : matrix{matrix_} {}

   Background(const std::unordered_map<char, charPoint> &images_map, std::string const &input_string);

   void add_point(charPoint *point, unsigned x, unsigned y);

   void add_point(const std::unordered_map<char, charPoint> &images_map, char ch, unsigned x, unsigned y);

   void add_row(std::array<const charPoint *, length> row, unsigned y);

   void add_row(const std::unordered_map<char, charPoint> &images_map, std::string row, unsigned y);

   void add_column(std::array<const charPoint *, height> column, unsigned x);

   void add_column(const std::unordered_map<char, charPoint> &images_map, std::string column, unsigned x);

   bool is_nullptr(Coord cd) const { return matrix[cd.y][cd.x] == nullptr; }

   charPoint const &get_point(int x, int y) const { return *matrix[y][x]; }  // do not call if is_nullptr==true
   charPoint const &get_point(Coord cd) const { return get_point(cd.x, cd.y); }  // do not call if is_nullptr==true

   bool is_solid(Coord cd) const { return is_solid(cd.x, cd.y); }

   bool is_solid(int x, int y) const { return get_point(static_cast<unsigned>(x), static_cast<unsigned>(y)).is_solid; }

   void print(WINDOW *window = stdscr) const;

   void print(WINDOW *window, Coord cd) const;
};

#endif //RPGGAME_BACKGROUND_H
