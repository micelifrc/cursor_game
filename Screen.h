//
// Created by mich on 10/11/18.
//

#ifndef RPGGAME_SCREEN_H
#define RPGGAME_SCREEN_H


// This is a structure to draw a full page in the terminal

#include <iostream>
#include <array>
#include "ncurses.h"

// screen size
#define height 55
#define length 205

enum struct Compass {
   north, east, south, west
};

struct Coord {
   int x, y;

   explicit Coord(int x_ = 0, int y_ = 0) : x{x_}, y{y_} {}

   explicit Coord(Compass cps);
};

struct charPoint {
   char output_char;
   unsigned color_code;
   bool is_solid;
   bool is_bold;

   charPoint() : output_char{' '}, color_code{0}, is_solid{false}, is_bold{false} {}

   explicit charPoint(char output_char_, unsigned color_code_, bool is_solid_ = false, bool is_blod_ = false)
         : output_char{output_char_}, color_code{color_code_}, is_solid{is_solid_}, is_bold{is_blod_} {}

   explicit charPoint(char ch);
};

void print(WINDOW *window, const charPoint &pt, unsigned y, unsigned x);

void print(WINDOW *window, const charPoint &pt, Coord cd = Coord());

template<typename Point = charPoint>  // should have a function print(WINDOW*, const Point&, const Coord)->void
struct Screen {
   std::array<std::array<Point, length>, height> matrix;

   Screen() = default;

   explicit Screen(std::array<std::array<Point, length>, height> matrix_) : matrix{matrix_} {}

   explicit Screen(std::string const &input_string);

   void add_point(Point point, unsigned x, unsigned y);

   void add_row(std::array<Point, length> row, unsigned y);

   void add_column(std::array<Point, height> column, unsigned x);

   Point &get_point(unsigned x, unsigned y) { return matrix[y][x]; }

   Point const &get_point(unsigned x, unsigned y) const { return matrix[y][x]; }
};

template<typename Point>
void print(WINDOW *window, const Screen<Point> &screen);

#include "Screen.tpp"

#endif //RPGGAME_SCREEN_H
