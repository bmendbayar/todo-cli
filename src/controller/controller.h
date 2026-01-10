#pragma once

#include <cstdlib>
#include <memory>

#include "actions.h"
#include "model.h"
#include "view.h"

namespace todo {
class Controller {
private:
  Model model_;                 ///< The [M]odel of the MVC.
  std::unique_ptr<View> view_;  ///< The [V]iew of the MVC.

  std::stack<std::unique_ptr<Action>> undo_stack_;  ///< Undo stack.
  std::stack<std::unique_ptr<Action>> redo_stack_;  ///< Redo stack.

public:
  /// \brief Default constructor.
  /// \note Based on args the view will be different.
  Controller(int argc, char **argv);

  /// \brief Starting point for program.
  void run();

private:
  /// \brief Handles the addition of a task.
  /// \param ch Character used for mode/menu selection.
  void handle_add(int ch);

  /// \brief Handles the removal of a task.
  void handle_remove();

  /// \brief Handles the output of the list.
  void handle_display();

  /// \brief Handles the emptying of the list.
  void handle_clear();

  /// \brief Handles the status change of a task.
  void handle_status_change();

  /// \brief Handles the priority change of a task.
  void handle_prio_change();

  /// \brief Handles the undo of an action.
  void handle_undo();

  /// \brief Handles the redo of an action.
  void handle_redo();

  /// \brief Parses a char number to a path vector.
  /// \param user_input The user_input to parse.
  /// \return A vector of non-negative integers.
  std::vector<u64> parse_path(const UserInput &user_input);
};
}  // namespace todo
