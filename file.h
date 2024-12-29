#ifndef FILE_H
#define FILE_H
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

class file {
public:
  std::vector<std::string> array;
  std::string file_name = "output.txt";
  unsigned int line_num = 0;
  unsigned int x_origin = 0;
  void init_screen();
  void refresh_screen();
  void refresh_line();
  void refresh_line(int line);
  void move_to(int event_y, int event_x);
  void backspace();
  void newline();
  void move_up();
  void move_down();
  void move_left();
  bool move_right();
  void edit(char new_ch);

  void read_file();
  void write_file();
};

#endif
