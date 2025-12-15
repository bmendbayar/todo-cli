#include <ncurses.h>

#include "vi_view.h"

Todo::ViView::ViView()
{
  initscr();
  cbreak();
  noecho();
  curs_set(1);

  scroll_offset_ = 0;
  list_pad_ = newpad(1000, COLS - 2);
}

Todo::ViView::~ViView()
{
  delwin(list_pad_);
}

void Todo::ViView::refresh_list_view()
{
  prefresh(list_pad_, scroll_offset_, 0, 2, 1, COLS, LINES);
}

std::string Todo::ViView::get_menu_opt()
{
  refresh_list_view();

  std::string buffer;
  int ch;
  curs_set(1);
  while (true)
  {
    ch = wgetch(list_pad_);

    if (ch == '\n')
    {
      break;
    }
    else if (ch == KEY_UP || ch == 'k')
    {
      if (scroll_offset_ > 0)
      {
        scroll_offset_--;
        refresh_list_view();
      }
    }
    else if (ch == KEY_DOWN || ch == 'j')
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

void Todo::ViView::display_list(const std::vector<Todo::Task> &todo_list, size_t level)
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

    mvwprintw(list_pad_, y, x, "%zu. %s %s [%s]\n", i + 1, status.c_str(),
              t.desc.c_str());

    if (!t.child_tasks.empty())
    {
      display_list(t.child_tasks, level + 1);
    }
  }

  refresh_list_view();
}

void Todo::ViView ::display_msg(const std::string &msg)
{
  mvwprintw(list_pad_, 0, COLS - msg.length() - 5, msg.c_str());
}

std::string Todo::ViView::get_input(const std::string &msg)
{
  return get_menu_opt();
}
