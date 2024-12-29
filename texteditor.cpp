#include "file.cpp"
#include "file.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 1) {
    std::cerr << "usage: ./texteditor <input_file>\n";
    return 1;
  }

  file main_file;
  main_file.file_name = argv[1];
  main_file.line_num = 0;
  main_file.init_screen();
  main_file.read_file();

  // Initialize Mouse Event Detection
  mousemask(BUTTON1_PRESSED, 0);
  MEVENT event;
  move(0, 0);

  int ch;
  while (true) {
    ch = getch();
    // std::cout << ch << "\n";
    if (ch != 259 && ch != 258) {
      main_file.x_origin = 0;
    }
    switch (ch) {
    case KEY_MOUSE:
      if (getmouse(&event) == OK) {
        if (event.bstate & BUTTON1_PRESSED) {
          main_file.move_to(event.y, event.x);
        }
      }
      break;
    case 259: // Up Arrow
      main_file.move_up();
      break;
    case 258: // Down Arrow
      main_file.move_down();
      break;
    case 260: // Left Arrow
      main_file.move_left();
      break;
    case 261: // Right Arrow
      main_file.move_right();
      break;
    case '\r':
    case '\n':
      main_file.newline();
      break;
    case 263: // Backspace
      main_file.backspace();
      break;
    case 330: // Delete
      if (main_file.move_right()) {
        main_file.backspace();
      }
      break;
    case 0x13: // Ctrl + s
      main_file.write_file();
      break;
    case 0x17: // Ctrl + w
      endwin();
      return 0;
    default:
      main_file.edit(ch);
      break;
    }
  }
}
