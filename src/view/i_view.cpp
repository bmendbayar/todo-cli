#include <ncurses.h>

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include "i_view.h"
#include "task.h"

namespace todo {
IView::IView()
{
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  if (has_colors() == false) {
    fprintf(stderr, "err: terminal has no color support\n");
    std::exit(EXIT_FAILURE);
  }
  start_color();

  int height = LINES;
  int width = COLS;
  int menu_height = 12;

  list_height_ = height - menu_height;
  list_width_ = width;
  scroll_offset_ = 0;

  list_pad_ = newpad(1000, width - 2);
  list_border_ = newwin(list_height_, width, 0, 0);
  menu_win_ = newwin(menu_height, width, list_height_, 0);

  keypad(menu_win_, true);

  wclear(list_border_);
  box(list_border_, 0, 0);
  mvwprintw(list_border_, 1, 2, "=== Todo List ===");
  wrefresh(list_border_);
}

IView::~IView()
{
  delwin(list_pad_);
  delwin(list_border_);
  delwin(menu_win_);
  endwin();
}

void IView::refresh_list_view()
{
  prefresh(list_pad_, scroll_offset_, 0, 2, 1, list_height_ - 2, list_width_ - 2);
}

UserInput IView::get_input(const std::string &msg)
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);

  std::stringstream ss(msg);
  std::string line;
  int curr_y = 1;
  int padding = 2;

  while (std::getline(ss, line)) {
    mvwprintw(menu_win_, curr_y, padding, "%s", line.c_str());
    curr_y++;
  }

  wrefresh(menu_win_);

  std::string buffer;
  int ch;

  curs_set(1);

  while (true) {
    mvwprintw(menu_win_, curr_y + 1, 2, "> ");
    mvwprintw(menu_win_, curr_y + 1, 4, "%s", std::string(buffer.length() + 5, ' ').c_str());
    mvwprintw(menu_win_, curr_y + 1, 4, "%s", buffer.c_str());
    wrefresh(menu_win_);

    ch = wgetch(menu_win_);

    if (ch == '\n') {
      break;
    } else if (ch == KEY_UP) {
      if (scroll_offset_ > 0) {
        scroll_offset_--;
        refresh_list_view();
      }
    } else if (ch == KEY_DOWN) {
      scroll_offset_++;
      refresh_list_view();
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
      if (!buffer.empty()) {
        buffer.pop_back();
      }
    } else if (isprint(ch)) {
      buffer.push_back(ch);
    }
  }

  curs_set(0);
  return {buffer, false};
}

void IView::display_list(const std::vector<Task> &todo_list, u16 level)
{
  if (level == 0) {
    wclear(list_pad_);
  }

  u16 lsize = todo_list.size();
  for (u16 i = 0; i < lsize; ++i) {
    const auto &t = todo_list[i];
    std::string status;
    switch (t.status) {
      case Status::NOT_STARTED:
        status = " ";
        break;
      case Status::IN_PROGRESS:
        status = "~";
        break;
      case Status::COMPLETED:
        status = "X";
        break;
      default:
        break;
    }

    int y = getcury(list_pad_);
    int x = 1 + (level * 2);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);   // low
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // medium
    init_pair(3, COLOR_RED, COLOR_BLACK);     // high
    if (t.priority < 30) {
      wattron(list_pad_, COLOR_PAIR(1));
    } else if (t.priority < 70) {
      wattron(list_pad_, COLOR_PAIR(2));
    } else {
      wattron(list_pad_, COLOR_PAIR(3));
    }

    mvwprintw(list_pad_, y, x, "%zu. [%s] %s\n", i + 1, status.c_str(), t.desc.c_str());

    if (!t.child_tasks.empty()) {
      display_list(t.child_tasks, level + 1);
    }
  }

  refresh_list_view();
}

void IView::display_msg(const std::string &msg)
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);
  mvwprintw(menu_win_, 2, 2, "%s", msg.c_str());
  mvwprintw(menu_win_, 4, 2, "press any key to continue ");
  wrefresh(menu_win_);
  wgetch(menu_win_);
}
}  // namespace todo
