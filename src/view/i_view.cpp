#include <ncurses.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "i_view.h"

namespace Todo
{
IView::IView()
{
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  int height = LINES;
  int width = COLS;
  int menu_height = 12;

  list_height_ = height - menu_height;
  list_width_ = width;
  scroll_offset_ = 0;

  list_pad_ = newpad(1000, width - 2);
  list_win_ = newwin(list_height_, width, 0, 0);
  menu_win_ = newwin(menu_height, width, list_height_, 0);

  keypad(menu_win_, true);

  wclear(list_win_);
  box(list_win_, 0, 0);
  mvwprintw(list_win_, 1, 2, "=== Todo List ===");
  wrefresh(list_win_);
}

IView::~IView()
{
  delwin(list_pad_);
  delwin(list_win_);
  delwin(menu_win_);
  endwin();
}

void IView::refresh_list_view()
{
  prefresh(list_pad_, scroll_offset_, 0, 2, 1, list_height_ - 2, list_width_ - 2);
}

std::string IView::get_input(const std::string &msg)
{
  wclear(menu_win_);
  box(menu_win_, 0, 0);

  std::stringstream ss(msg);
  std::string line;
  int curr_y = 1;
  int padding = 2;

  while (std::getline(ss, line))
  {
    mvwprintw(menu_win_, curr_y, padding, "%s", line.c_str());
    curr_y++;
  }

  wrefresh(menu_win_);

  std::string buffer;
  int ch;

  curs_set(1);

  while (true)
  {
    mvwprintw(menu_win_, curr_y + 1, 2, "> ");
    mvwprintw(menu_win_, curr_y + 1, 4, "%s",
              std::string(buffer.length() + 5, ' ').c_str());
    mvwprintw(menu_win_, curr_y + 1, 4, "%s", buffer.c_str());
    wrefresh(menu_win_);

    ch = wgetch(menu_win_);

    if (ch == '\n')
    {
      break;
    }
    else if (ch == KEY_UP)
    {
      if (scroll_offset_ > 0)
      {
        scroll_offset_--;
        refresh_list_view();
      }
    }
    else if (ch == KEY_DOWN)
    {
      scroll_offset_++;
      refresh_list_view();
    }
    else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
    {
      if (!buffer.empty())
      {
        buffer.pop_back();
      }
    }
    else if (isprint(ch))
    {
      buffer.push_back(ch);
    }
  }

  curs_set(0);
  return buffer;
}

void IView::display_list(const std::vector<Todo::Task> &todo_list, size_t level)
{
  if (level == 0)
  {
    wclear(list_pad_);
  }

  size_t lsize = todo_list.size();
  for (size_t i = 0; i < lsize; ++i)
  {
    const auto &t = todo_list[i];
    std::string status = [&t]() -> std::string
    {
      switch (t.status)
      {
        case Status::NOT_STARTED:
          return "[ ]";
        case Status::IN_PROGRESS:
          return "[-]";
        case Status::COMPLETED:
          return "[X]";
        default:
          return "[?]";
      }
    }();

    int y = getcury(list_pad_);
    int x = 1 + (level * 2);
    std::string prio = [&t]() -> std::string
    {
      if (t.prio > 80)
      {
        return "High";
      }
      else if (t.prio > 20)
      {
        return "Medium";
      }
      else
      {
        return "Low";
      }
    }();

    mvwprintw(list_pad_, y, x, "%zu. %s %s [%s]\n", i + 1, status.c_str(), t.desc.c_str(),
              prio.c_str());

    if (!t.child_tasks.empty())
    {
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
}  // namespace Todo
