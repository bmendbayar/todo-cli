#include <iostream>
#include <string>
#include <vector>

#include "view.h"

void Todo::View::display_list(const std::vector<Todo::Task> todo_list)
{
  for (const auto &task : todo_list)
  {
    std::cout << task.id << ". " << task.desc << ' '
              << (task.completion ? "(done)" : "(in progress)") << '\n';
  }
}

void Todo::View::display_msg(const std::string &msg)
{
  std::cout << msg << '\n';
}
