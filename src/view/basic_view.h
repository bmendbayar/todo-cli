#pragma once

#include "view.h"

namespace Todo
{
class BasicView : public View
{
public:
  /**
   * @brief Default constructor.
   */
  BasicView() = default;

  /**
   * @brief Gets input from the user.
   *
   * @param msg String to display.
   */
  virtual std::string get_input(const std::string &msg) override;

  /**
   * @brief Display @p task list to screen.
   *
   * @param todo_list Const reference to a vector.
   * @param level Recursion depth.
   */
  virtual void display_list(const std::vector<Todo::Task> &todo_list,
                            size_t level = 0) override;

  /**
   * @brief Display @p message to screen.
   *
   * @param msg String to display.
   */
  virtual void display_msg(const std::string &msg) override;
};
}  // namespace Todo
