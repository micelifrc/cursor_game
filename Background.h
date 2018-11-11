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
   north = 0, east = 1, south =2 , west = 3
};

Compass make_compass(unsigned n);

struct Coord {
   int x, y;

   explicit Coord(int x_ = 0, int y_ = 0) : x{x_}, y{y_} {}

   explicit Coord(Compass cps);
};

Coord make_coord_through_compass(unsigned n);

struct charPoint {
   char output_char;
   unsigned color_code;
   bool is_solid;
   bool is_bold;

   explicit charPoint(char output_char_ = ' ', unsigned color_code_ = 0, bool is_solid_ = false, bool is_blod_ = false)
         : output_char{output_char_}, color_code{color_code_}, is_solid{is_solid_}, is_bold{is_blod_} {}
};

void print(WINDOW *window, const charPoint &pt, int y, int x);

void print(WINDOW *window, const charPoint &pt, Coord cd = Coord());

struct Background {
   std::array<std::array<const charPoint*, length>, height> matrix;

   Background();

   explicit Background(const std::unordered_map<char, charPoint> &images_map);

   explicit Background(std::array<std::array<const charPoint*, length>, height> matrix_) : matrix{matrix_} {}

   Background(const std::unordered_map<char, charPoint> &images_map, std::string const &input_string);

   void add_point(charPoint* point, unsigned x, unsigned y);
   void add_point(const std::unordered_map<char, charPoint> &images_map, char ch, unsigned x, unsigned y);

   void add_row(std::array<const charPoint*, length> row, unsigned y);
   void add_row(const std::unordered_map<char, charPoint> &images_map, std::string row, unsigned y);

   void add_column(std::array<const charPoint*, height> column, unsigned x);
   void add_column(const std::unordered_map<char, charPoint> &images_map, std::string column, unsigned x);

   charPoint const &get_point(unsigned x, unsigned y) const { return *matrix[y][x]; }
};

void print(WINDOW *window, const Background &bg);

#endif //RPGGAME_BACKGROUND_H
