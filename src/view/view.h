#pragma once

#include <string>
#include <vector>

#include "model.h"

namespace Todo
{
enum class MenuOptions
{
  EXIT = 0,
  ADD = 1,
  REMOVE = 2,
  CHANGE_STATUS = 3,
  CHANGE_PRIO = 4,
  CLEAR = 5,
  INVALID = 6
};

class View
{
protected:
  View() = default;

public:
  virtual ~View() = default;

  /**
   * @brief Gets input from the user.
   *
   * @param msg String to display.
   */
  virtual std::string get_input(const std::string &msg) = 0;

  /**
   * @brief Display @p task list to screen.
   *
   * @param todo_list Const reference to a vector.
   * @param level Recursion depth.
   */
  virtual void display_list(const std::vector<Todo::Task> &todo_list,
                            size_t level = 0) = 0;

  /**
   * @brief Display @p message to screen.
   *
   * @param msg String to display.
   */
  virtual void display_msg(const std::string &msg) = 0;
};
}  // namespace Todo
