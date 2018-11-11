template<typename Point>
Screen<Point>::Screen(std::string const &input_string) {
   for (unsigned y = 0; y != height; ++y) {
      for (unsigned x = 0; x != length; ++x) {
         matrix[y][x] = Point(input_string[y * length + x]);
      }
   }
}

template<typename Point>
void Screen<Point>::add_point(Point point, unsigned x, unsigned y) {
   matrix[y][x] = Point(point);
}

template<typename Point>
void Screen<Point>::add_row(std::array<Point, length> row, unsigned y) {
   for (unsigned x = 0; x != length; ++x)
      matrix[y][x] = Point(row[x]);
}

template<typename Point>
void Screen<Point>::add_column(std::array<Point, height> column, unsigned x) {
   for (unsigned y = 0; y != height; ++y)
      matrix[y][x] = Point(column[x]);
}

template<typename Point>
void print(WINDOW *window, const Screen<Point> &screen) {
   wclear(window);
   for (unsigned y = 0; y != height; ++y) {
      for (unsigned x = 0; x != length; ++x) {
         print(window, screen.get_point(x, y), x, y);
      }
   }
   curs_set(0);
   noecho();
   cbreak();
   wrefresh(window);
   int gtc = getch();
}
