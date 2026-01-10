#include <ncurses.h>

#include <iostream>
#include <limits>
#include <string>

#include "basic_view.h"
#include "task.h"
#include "types.h"
#include "user_input.h"

namespace todo {
void clear_input_buf()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

UserInput BasicView::get_input(const std::string &msg)
{
  std::string buffer;
  std::cout << msg;
  std::getline(std::cin, buffer);
  clear_input_buf();
  return {buffer, false};
}

void BasicView::display_list(const std::vector<Task> &todo_list, u16 level)
{
  u16 id = 1;
  for (const auto &t : todo_list) {
    std::string status = [&t]() -> std::string {
      switch (t.status) {
        case Status::NOT_STARTED:
          return " ";
        case Status::IN_PROGRESS:
          return "~";
        case Status::COMPLETED:
          return "X";
        default:
          return "";
      }
    }();

    std::cout << std::string(level, ' ') << id << ". [" << status << "] " << t.desc << '\n';

    if (todo_list[id - 1].child_tasks.size() != 0) {
      display_list(todo_list[id - 1].child_tasks, level + 1);
    }

    ++id;
  }

  if (level == 0) {
    std::cout << '\n';
  }
}

void BasicView::display_msg(const std::string &msg)
{
  std::cout << msg << '\n';
}
}  // namespace todo
