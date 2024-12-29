#include "file.h"

void file::init_screen() {
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE); // function keys reading enabled
  curs_set(2);
}

void file::read_file() {
  std::ifstream input_file(file_name);
  array.clear();
  int i = 0;
  for (std::string line; getline(input_file, line);) {
    array.push_back(line + '\n');
  }
  refresh_screen();
}

void file::write_file() {
  std::ofstream output_file;
  output_file.open(file_name);
  for (int i = 0; i < array.size(); i++) {
    output_file << array[i];
  }
  output_file.close();
}

void file::refresh_screen() {
  clear();
  for (int y = 0; y < array.size(); y++) {
    for (int x = 0; x < array[y].size(); x++) {
      addch(array[y][x]);
    }
  }
  refresh();
}

void file::refresh_line() {
  move(line_num, 0);
  for (int x = 0; x < array[line_num].size(); x++) {
    addch(array[line_num][x]);
  }
  if (array[line_num].back() != '\n') {
    clrtoeol();
  }
  refresh();
}

void file::refresh_line(int line) {
  move(line, 0);
  for (int x = 0; x < array[line].size(); x++) {
    addch(array[line][x]);
  }
  if (array[line_num].back() != '\n') {
    clrtoeol();
  }
  refresh();
}

void file::move_to(int event_y, int event_x) {
  int y, x;
  getyx(stdscr, y, x);

  // If click longer than length of file, move to bottom of file
  if (event_y >= array.size()) {
    line_num = array.size() - 1;
    wmove(stdscr, line_num, x);
  } else {
    line_num = event_y;
    wmove(stdscr, event_y, x);
  }

  getyx(stdscr, y, x);

  // If click longer than length of line, move to end of line
  if (event_x >= array[line_num].size()) {
    short newline_offset = 0;
    if (array[line_num].back() == '\n') {
      newline_offset = -1;
    }
    wmove(stdscr, y, array[line_num].size() + newline_offset);
  } else {
    wmove(stdscr, y, event_x);
  }
}

void file::move_up() {
  int y, x;
  getyx(stdscr, y, x);

  if (!y) {
    return;
  }
  if (x > x_origin) {
    x_origin = x;
  }
  move_to(y - 1, x_origin);
}

void file::move_down() {
  int y, x;
  getyx(stdscr, y, x);

  if (x > x_origin) {
    x_origin = x;
  }
  move_to(y + 1, x_origin);
}

void file::move_left() {
  int y, x;
  getyx(stdscr, y, x);

  if (!x && !y) {
    return;
  }
  if (!x) {
    line_num--;
    move(y - 1, array[line_num].size() - 1);
  } else {
    move(y, x - 1);
  }
}

bool file::move_right() {
  int y, x;
  getyx(stdscr, y, x);

  short newline_offset = 0;
  if (array[line_num].back() == '\n') {
    newline_offset = -1;
  }
  if (x == array[line_num].size() + newline_offset) {
    if (y + 1 == array.size()) {
      return 0;
    }
    line_num++;
    move(y + 1, 0);
    return 1;
  } else {
    move(y, x + 1);
    return 1;
  }
}

void file::backspace() {
  int y, x;
  getyx(stdscr, y, x);
  if (!y && !x)
    return;

  if (!x) {
    line_num--;
    y--;
    x = array[line_num].size();
    if (array[line_num + 1].size()) {
      array[line_num] += array[line_num + 1];
      array[line_num + 1].clear();
    }
  }
  array[line_num].erase(x - 1, 1);
  refresh_screen();
  move(y, x - 1);
}

void file::newline() {
  int y, x;
  getyx(stdscr, y, x);

  if (x == array[line_num].size()) {
    array[line_num].push_back('\n');
    refresh_line();
    array.push_back("");
  } else {
    std::string rest_of_line =
        array[line_num].substr(x, array[line_num].size() - x);
    array[line_num].erase(x, array[line_num].size() - x).push_back('\n');
    refresh_line();
    auto pos = array.begin();

    array.insert(std::next(pos, line_num + 1), 1, rest_of_line);
  }
  line_num++;
  refresh_screen();
  move(y + 1, 0);
}

void file::edit(char new_ch) {
  int y, x;
  getyx(stdscr, y, x);
  array[line_num].insert(x, 1, new_ch);
  refresh_line();
  move(y, x + 1);
}