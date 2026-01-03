#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>

#include "actions.h"
#include "basic_view.h"
#include "controller.h"
#include "i_view.h"
#include "menu_opts.h"
#include "vi_view.h"
#include "view.h"

namespace Todo {
constexpr std::string SEN = "deadbeef";

Controller::Controller(int argc, char **argv)
{
  if (argc == 1) {
    view_ = std::make_unique<ViView>();
  } else if (strcmp(argv[1], "-b") == 0) {
    view_ = std::make_unique<BasicView>();
  } else if (strcmp(argv[1], "-i") == 0) {
    view_ = std::make_unique<IView>();
  }
}

void Controller::run()
{
  bool running = true;
  while (running) {
    handle_display();
    UserInput str_opt = view_->get_input(
      "=== Todo Menu ===\n"
      "1. Add task\n"
      "2. Remove task\n"
      "3. Change task status\n"
      "4. Change task priority\n"
      "5. Clear list\n"
      "0. Exit\n");

    MenuOptions opt;
    if (str_opt.vi_mode == true) {
      if (str_opt.text == "d") {
        opt = MenuOptions::REMOVE;
      } else if (str_opt.text == "x") {
        opt = MenuOptions::CHANGE_PRIO;
      } else if (str_opt.text == "c") {
        opt = MenuOptions::CHANGE_STATUS;
      } else if (str_opt.text == "q") {
        opt = MenuOptions::EXIT;
      } else if (str_opt.text == "o" || str_opt.text == "O") {
        opt = MenuOptions::ADD;
      } else if (str_opt.text == "u") {
        opt = MenuOptions::UNDO;
      } else if (str_opt.text == "g") {
        opt = MenuOptions::REDO;
      } else {
        opt = MenuOptions::INVALID;
      }
    } else {
      opt = static_cast<MenuOptions>(std::stoul(str_opt.text));
    }

    switch (opt) {
      case MenuOptions::ADD:
        handle_add(str_opt.text[0]);
        break;

      case MenuOptions::REMOVE:
        handle_remove();
        break;

      case MenuOptions::CHANGE_STATUS:
        handle_status_change();
        break;

      case MenuOptions::CHANGE_PRIO:
        handle_prio_change();
        break;

      case MenuOptions::UNDO:
        handle_undo();
        break;

      case MenuOptions::REDO:
        handle_redo();
        break;

      case MenuOptions::CLEAR:
        handle_clear();
        break;

      case MenuOptions::EXIT:
        running = false;
        model_.save_file();
        break;

      case MenuOptions::INVALID:
      default:
        view_->display_msg("Invalid option. Please try again.");
        break;
    }
  }
}

inline bool pre_order_trav(const std::vector<Task> &list, int &curr, const int target,
                           std::vector<u16> &path)
{
  for (u16 i{}; i < list.size(); i++) {
    path.push_back(i);
    curr++;

    if (curr == target) {
      return true;
    }

    if (list[i].child_tasks.empty() == false) {
      if (pre_order_trav(list[i].child_tasks, curr, target, path)) {
        return true;
      }
    }

    path.pop_back();
  }

  return false;
}

std::vector<u16> Controller::parse_path(const UserInput &user_input)
{
  std::vector<u16> path;

  if (user_input.vi_mode == false) {
    for (const char &c : user_input.text) {
      if (isdigit(c)) {
        path.emplace_back(c - '0' - 1);
      } else {
        throw std::runtime_error("Error: Path can only be numbers");
        return {};
      }
    }
  } else {
    int x{};
    pre_order_trav(model_.get_list(), x, std::stoul(user_input.text), path);
  }

  return path;
}

void Controller::handle_add(int ch)
{
  try {
    auto desc = view_->get_input("Enter the description of your task: ");
    auto path = view_->get_input("Enter the path of the new task: ");
    auto prio = view_->get_input("Enter the priority of the task (1-100): ");

    std::vector<u16> vpath = parse_path(path);
    if (ch == 'O' && vpath.empty() == false) {
      vpath.pop_back();
    }

    if (path.text == SEN || prio.text == SEN || desc.text == SEN) {
      return;
    }

    if (std::stoul(prio.text) > 100) {
      view_->display_msg("priority can only be 1-100");
      return;
    }

    Task task{desc.text, {}, (u16)std::stoul(prio.text), {}};
    auto action = std::make_unique<AddAction>(model_, vpath, task);
    action->execute();
    undo_stack_.push(std::move(action));

    while (redo_stack_.empty() == false) {
      redo_stack_.pop();
    }
  } catch (const std::out_of_range &e) {
    view_->display_msg("Error: Out of range access");
  } catch (const std::exception &e) {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_remove()
{
  try {
    UserInput path_str = view_->get_input("Enter the path of the task: ");
    if (path_str.text == SEN) {
      return;
    }

    auto action = std::make_unique<RemoveAction>(model_, parse_path(path_str));
    action->execute();
    undo_stack_.push(std::move(action));

    while (redo_stack_.empty() == false) {
      redo_stack_.pop();
    }
  } catch (const std::out_of_range &e) {
    view_->display_msg("Error: Out of range access");
  } catch (const std::exception &e) {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_display()
{
  view_->display_list(model_.get_list());
}

void Controller::handle_clear()
{
  model_.clear();
}

void Controller::handle_status_change()
{
  try {
    UserInput path_str = view_->get_input("Enter the path of the task: ");
    UserInput status = view_->get_input("Which status (1-NS, 2-IP, 3-FIN): ");
    if (path_str.text == SEN || status.text == SEN) {
      return;
    }

    auto action = std::make_unique<StatusChangeAction>(
      model_, parse_path(path_str), static_cast<Status>(std::stoul(status.text)));
    action->execute();
    undo_stack_.push(std::move(action));

    while (redo_stack_.empty() == false) {
      redo_stack_.pop();
    }
  } catch (const std::out_of_range &e) {
    view_->display_msg("Error: Out of range access");
  } catch (const std::exception &e) {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_prio_change()
{
  try {
    UserInput path_str = view_->get_input("Enter the path of the task: ");
    UserInput prio = view_->get_input("Which priority (1-L, 2-M, 3-H): ");
    if (path_str.text == SEN || prio.text == SEN) {
      return;
    }

    auto action = std::make_unique<PriorityChangeAction>(model_, parse_path(path_str),
                                                         std::stoul(prio.text));
    action->execute();
    undo_stack_.push(std::move(action));

    while (redo_stack_.empty() == false) {
      redo_stack_.pop();
    }
  } catch (const std::out_of_range &e) {
    view_->display_msg("Error: Out of range access");
  } catch (const std::exception &e) {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_undo()
{
  if (undo_stack_.empty() == true) {
    return;
  }

  undo_stack_.top()->undo();
  redo_stack_.push(std::move(undo_stack_.top()));
  undo_stack_.pop();
}

void Controller::handle_redo()
{
  if (redo_stack_.empty() == true) {
    return;
  }

  redo_stack_.top()->execute();
  undo_stack_.push(std::move(redo_stack_.top()));
  redo_stack_.pop();
}
}  // namespace Todo
