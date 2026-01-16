#pragma once

#include <ncurses.h>

#include "types.h"
#include "view.h"

namespace todo {
class ViView : public View {
private:
  WINDOW *list_pad_{nullptr};  ///< Curses pad to display list.
  WINDOW *notif_{nullptr};     ///< Curses window to display notifications/messages.

  struct {
    u16 x{};
    u16 y{};
  } cursor_;  ///< Curses cursor location.

  int border_y_{};       ///< Y-bound of list_pad_.
  int border_x_{};       ///< X-bound of list_pad_.
  int scroll_offset_{};  ///< For scrolling convenience.

  enum class Mode : u8 {
    NORMAL = 0,
    REMOVE = 1,
    CHANGE = 2,
    SIBLING_INSERT = 3,
    CHILD_INSERT = 4,
  } mode_{Mode::NORMAL};  ///< Current mode the view is in.

  enum class InsertChain : u8 {
    DESC = 0,
    PATH = 1,
    PRIORITY = 2,
    DATE = 3,
  } curr_event_{InsertChain::DESC};  ///< The chain of events for a task insertion.

public:
  /// \brief Default constructor.
  ViView();

  /// \brief Destructor.
  ~ViView();

  /// \brief Gets input from the user.
  /// \param msg String to display.
  virtual UserInput get_input(const std::string &msg) override;

  /// \brief Display task list to screen.
  /// \param todo_list Const reference to a vector.
  /// \param level Recursion depth.
  virtual void display_list(const std::vector<Task> &todo_list, u16 level = 0) override;

  /// \brief Display message to screen.
  /// \param msg String to display.
  virtual void display_msg(const std::string &msg) override;

private:
  /// \brief Refresh list_pad_.
  void refresh_list_view();

  /// \brief Handles NORMAL mode.
  UserInput handle_normal();

  /// \brief Handles REMOVE mode.
  UserInput handle_remove();

  /// \brief Handles CHANGE mode.
  UserInput handle_change();

  /// \brief Handles INSERT mode.
  UserInput handle_insert();

  /// \brief Handles CHILD_INSERT mode.
  void handle_child_insert();

  /// \brief Handles SIBLING_INSERT mode.
  void handle_sibling_insert();
};
}  // namespace todo
