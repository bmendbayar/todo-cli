#pragma once

#include <cstdlib>

#include "model.h"
#include "view.h"

namespace Todo
{
class Controller
{
private:
  Model model_;  ///< The [M]odel of the MVC.
  View *view_;   ///< The [V]iew of the MVC.

public:
  /**
   * @brief Default constructor.
   * @note based on args the view will be different.
   */
  Controller(int argc, char **argv);

  /**
   * @brief Starting point for program.
   */
  void run();

private:
  /**
   * @brief Handles the addition of a task.
   */
  void handle_add();

  /**
   * @brief Handles the removal of a task.
   */
  void handle_remove();

  /**
   * @brief Handles the output of the list.
   */
  void handle_display();

  /**
   * @brief Handles the emptying of the list.
   */
  void handle_clear();

  /**
   * @brief Handles the status change of a task.
   */
  void handle_status_change();

  /**
   * @brief Handles the priority change of a task.
   */
  void handle_prio_change();

  /**
   * @brief Parses a string of numbers to a vector.
   *
   * @param spath The string to parse.
   * @return A vector of positive integers.
   */
  std::vector<size_t> parse_path(const std::string &spath);
};
}  // namespace Todo
