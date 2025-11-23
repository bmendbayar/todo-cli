#pragma once

#include <cstdlib>
#include <string>

#include "model.h"
#include "view.h"

namespace Todo
{
class Controller
{
private:
  Model model_;
  View view_;

public:
  Controller() = default;

  void run();

private:
  int get_input();

  void pass_add(const std::string &todo);

  void pass_rm(size_t index);

  void pass_display();
};
}  // namespace Todo
