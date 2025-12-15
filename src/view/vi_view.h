#pragma once

#include <ncurses.h>

#include "view.h"

namespace Todo
{
class ViView : public View
{
private:
  WINDOW *list_pad_;
  int scroll_offset_;

public:
  /**
   * @brief Default constructor.
   */
  ViView();

  /**
   * @brief Destructor.
   */
  ~ViView();

  /**
   * @brief Gets input from the user.
   *
   * @return User's string input.
   */
  virtual std::string get_menu_opt() override;

  /**
   * @brief Gets input from the user.
   *
   * @param msg String to display.
   * @return User's string input.
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

private:
  void refresh_list_view();
};
}  // namespace Todo
