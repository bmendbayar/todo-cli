#include <curses.h>

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "basic_view.h"

void clear_input_buf()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string Todo::BasicView::get_input(const std::string &msg)
{
  std::string buffer;
  std::cout << msg;
  std::getline(std::cin, buffer);
  clear_input_buf();
  return buffer;
}

void Todo::BasicView::display_list(const std::vector<Todo::Task> &todo_list, size_t level)
{
  size_t id = 1;
  for (const auto &t : todo_list)
  {
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
          return "[404]";
      }
    }();

    std::cout << std::string(level, ' ') << id << ". " << status << ' ' << t.desc << '\n';

    if (todo_list[id - 1].child_tasks.size() != 0)
    {
      display_list(todo_list[id - 1].child_tasks, level + 1);
    }

    ++id;
  }
  std::cout << '\n';
}

void Todo::BasicView::display_msg(const std::string &msg)
{
  std::cout << msg << '\n';
}
