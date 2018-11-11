//
// Created by mich on 10/11/18.
//

#include "Background.h"

Compass make_compass(unsigned n){
   switch (n % 4){
      case 0 : return Compass::north;
      case 1 : return Compass::east;
      case 2 : return Compass::south;
      case 3 : return Compass::west;
   }
}

Coord::Coord(const Compass cps) {
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

Coord &Coord::operator+=(const Coord &rhs) {
   x += rhs.x;
   y += rhs.y;
   return *this;
}

Coord &Coord::operator-=(const Coord &rhs) {
   x -= rhs.x;
   y -= rhs.y;
   return *this;
}

Coord make_coord_through_compass(unsigned n) {
   return Coord(Compass(n));
}

void charPoint::print(WINDOW *window, int x, int y) const {
   wattron(window, COLOR_PAIR(color_code));
   if (is_bold) wattron(window, A_BOLD);
   mvwprintw(window, y, x, "%c", output_char);
   if (is_bold) wattroff(window, A_BOLD);
   wattroff(window, COLOR_PAIR(color_code));
}

Background::Background() {
   for(auto & row: matrix){
      for(auto point: row){
         point = nullptr;
      }
   }
}

Background::Background(const std::unordered_map<char, charPoint> &images_map){
   for(auto & row: matrix){
      for(auto point: row){
         point = &images_map.at('a');
      }
   }
}

Background::Background(const std::unordered_map<char, charPoint> &images_map, std::string const &input_string) {
   for (unsigned y = 0; y != height; ++y) {
      for (unsigned x = 0; x != length; ++x) {
         matrix[y][x] = &images_map.at(input_string[y * length + x]);
      }
   }
}

void Background::add_point(charPoint *point, unsigned x, unsigned y) {
   matrix[y][x] = point;
}

void Background::add_row(std::array<const charPoint*, length> row, unsigned y) {
   for (unsigned x = 0; x != length; ++x)
      matrix[y][x] = row[x];
}

void Background::add_column(std::array<const charPoint*, height> column, unsigned x) {
   for (unsigned y = 0; y != height; ++y)
      matrix[y][x] = column[x];
}

void Background::add_point(const std::unordered_map<char, charPoint> &images_map, char ch, unsigned x, unsigned y) {
   matrix[y][x] = &images_map.at(ch);
}

void Background::add_row(const std::unordered_map<char, charPoint> &images_map, std::string row, unsigned y) {
   for (unsigned x = 0; x != length; ++x) add_point(images_map, row[x], x, y);
}

void Background::add_column(const std::unordered_map<char, charPoint> &images_map, std::string column, unsigned x) {
   for (unsigned y = 0; y != height; ++y) add_point(images_map, column[y], x, y);
}

void Background::print(WINDOW *window) const {
   for (unsigned y = 0; y != height; ++y) {
      for (unsigned x = 0; x != length; ++x) {
         print(window, Coord(x, y));
      }
   }
}

void Background::print(WINDOW *window, Coord cd) const {
   if(is_nullptr(cd)){
      mvwprintw(window, cd.y, cd.x, " ");
   }else {
      get_point(cd).print(window, cd.x, cd.y);
   }
}