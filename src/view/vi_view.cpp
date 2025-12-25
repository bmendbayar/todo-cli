#include <string>

#include "vi_view.h"

namespace Todo
{
ViView::ViView()
{
  initscr();
  cbreak();
  noecho();
  curs_set(1);

  list_pad_ = newpad(1000, COLS);
}

ViView::~ViView()
{
  delwin(list_pad_);
  endwin();
}

void ViView::refresh_list_view()
{
  prefresh(list_pad_, border_y_, border_x_, 0, 0, 20, COLS);
}

UserInput ViView::get_input(const std::string &msg)
{
  std::string buffer;
  int ch;

  while (true)
  {
    ch = wgetch(list_pad_);
    if (mode_ == Mode::NORMAL)
    {
      /* cursor movement */
      if (ch == 'h' && cursor_.x > 0)
      {
        cursor_.x--;
      }
      else if (ch == 'j' && cursor_.y < 1000)
      {
        cursor_.y++;
      }
      else if (ch == 'k' && cursor_.y > 0)
      {
        cursor_.y--;
      }
      else if (ch == 'l' && cursor_.x < COLS)
      {
        cursor_.x++;
      }
      else if (ch == 'q' || ch == 'd' || ch == 'c' || ch == 'x')
      { /* d - delete, c - status change, x - prio change */
        mode_ = Mode::ABNORMAL;
        return {std::string(1, ch), true};
      }

      if (cursor_.y == getmaxy(stdscr) + scroll_offset_ - 7)
      {
        scroll_offset_++;
        border_y_++;
      }
      else if (cursor_.y == scroll_offset_ + 6)
      {
        scroll_offset_--;
        border_y_--;
      }

      wmove(list_pad_, cursor_.y, cursor_.x);
      refresh_list_view();
    }
    else
    {
      mvwprintw(list_pad_, 10, 10, "abnormal");
      if (ch == 'd' || ch == 'c' || ch == 'x')
      { /* d - delete, c - status change, x - prio change */
        display_msg(std::to_string(cursor_.y + 1));
        mode_ = Mode::NORMAL;
        return {std::to_string(cursor_.y + 1), true};
      }
      else if (ch == 'q')
      {
        return {"0", true};
      }

      wmove(list_pad_, cursor_.y, cursor_.x);
      refresh_list_view();
    }
  }

  return {{}, true};
}

void ViView::display_list(const std::vector<Task> &todo_list, U16 level)
{
  if (level == 0)
  {
    wclear(list_pad_);
  }

  U16 lsize = todo_list.size();
  for (U16 i = 0; i < lsize; ++i)
  {
    std::string status;
    if (todo_list[i].status == Status::NOT_STARTED)
      status = "[ ]";
    else if (todo_list[i].status == Status::IN_PROGRESS)
      status = "[-]";
    else if (todo_list[i].status == Status::COMPLETED)
      status = "[X]";
    else
      status = "[?]";

    int y = getcury(list_pad_);
    int x = 1 + (level * 2);

    std::string prio;
    if (todo_list[i].prio > 80)
      prio = "High";
    else if (todo_list[i].prio > 20)
      prio = "Medium";
    else
      prio = "Low";

    mvwprintw(list_pad_, y, x, "%zu. %s %s [%s]\n", i + 1, status.c_str(),
              todo_list[i].desc.c_str(), prio.c_str());

    if (!todo_list[i].child_tasks.empty())
    {
      display_list(todo_list[i].child_tasks, level + 1);
    }
  }

  wmove(list_pad_, cursor_.y, cursor_.x);
  refresh_list_view();
}

void ViView::display_msg(const std::string &msg)
{
  mvwprintw(list_pad_, 0, COLS - msg.size() - 2, "%s", msg.c_str());
  mvwprintw(list_pad_, 1, COLS - 29, "Press any key to continue. ");
  wmove(list_pad_, cursor_.y, cursor_.x);
  refresh_list_view();
  wgetch(list_pad_);
}
}  // namespace Todo
