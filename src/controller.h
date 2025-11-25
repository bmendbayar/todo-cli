#pragma once

#include <cstdlib>

#include "model.h"
#include "view.h"

namespace Todo
{
class Controller
{
private:
  Model model_;
  View *view_;

public:
  Controller();
  void run();

private:
  void handle_add();
  void handle_remove();
  void handle_display();
  void handle_status_change();
};
}  // namespace Todo
