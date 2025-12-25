#pragma once

#include <ncurses.h>

#include "types.h"
#include "view.h"

namespace Todo {
class ViView : public View {
private:
  WINDOW *list_pad_;
  struct {
    U16 x{};
    U16 y{};
  } cursor_;

  int border_y_{};
  int border_x_{};

  int scroll_offset_{};
  enum class Mode : U8 {
    NORMAL = 0,
    ABNORMAL = 1,
  } mode_ = Mode::NORMAL;

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
   * @param msg String to display.
   */
  virtual UserInput get_input(const std::string &msg) override;

  /**
   * @brief Display @p task list to screen.
   *
   * @param todo_list Const reference to a vector.
   * @param level Recursion depth.
   */
  virtual void display_list(const std::vector<Task> &todo_list, U16 level = 0) override;

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
