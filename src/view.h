#pragma once

#include <string>
#include <vector>

#include "model.h"

namespace Todo
{
class View
{
public:
  View() = default;

  void display_list(const std::vector<Todo::Task> todo_list);

  void display_msg(const std::string &msg);
};
}  // namespace Todo
