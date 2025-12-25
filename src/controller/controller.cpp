#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>

#include "basic_view.h"
#include "controller.h"
#include "i_view.h"
#include "menu_opts.h"
#include "vi_view.h"
#include "view.h"

namespace Todo
{
Controller::Controller(int argc, char **argv)
{
  if (argc == 1)
  {
    view_ = new IView();
  }
  else if (strcmp(argv[1], "-b") == 0)
  {
    view_ = new BasicView();
  }
  else if (strcmp(argv[1], "-vi") == 0)
  {
    view_ = new ViView();
  }
}

void Controller::run()
{
  bool running = true;
  while (running)
  {
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
    if (str_opt.vi_mode == true)
    {
      if (str_opt.text == "d")
        opt = MenuOptions::REMOVE;
      else if (str_opt.text == "x")
        opt = MenuOptions::CHANGE_PRIO;
      else if (str_opt.text == "c")
        opt = MenuOptions::CHANGE_STATUS;
      else if (str_opt.text == "q")
        opt = MenuOptions::EXIT;
      else
        opt = MenuOptions::INVALID;
    }
    else
    {
      opt = static_cast<MenuOptions>(std::stoul(str_opt.text));
    }

    switch (opt)
    {
      case MenuOptions::ADD:
        handle_add();
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

      case MenuOptions::CLEAR:
        handle_clear();
        break;

      case MenuOptions::EXIT:
        running = false;
        delete view_;
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
                           std::vector<U16> &path)
{
  for (U16 i{}; i < list.size(); i++)
  {
    path.push_back(i);
    curr++;

    if (curr == target)
      return true;

    if (list[i].child_tasks.empty() == false)
      if (pre_order_trav(list[i].child_tasks, curr, target, path))
        return true;

    path.pop_back();
  }

  return false;
}

std::vector<U16> Controller::parse_path(const UserInput &user_input)
{
  std::vector<U16> path;

  if (user_input.vi_mode == false)
  {
    for (const char &c : user_input.text)
    {
      if (isdigit(c))
      {
        path.emplace_back(c - '0' - 1);
      }
      else
      {
        throw std::runtime_error("Error: Path can only be numbers");
        return {};
      }
    }
  }
  else
  {
    int x{};
    pre_order_trav(model_.get_list(), x, std::stoul(user_input.text), path);
  }

  return path;
}

void Controller::handle_add()
{
  try
  {
    UserInput desc = view_->get_input("Enter the description of your task: ");
    UserInput str_path = view_->get_input("Enter the path of the new task: ");
    UserInput prio = view_->get_input("Enter the priority of the task (1-100): ");

    model_.add(desc.text, std::stoul(prio.text), parse_path(str_path));
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_remove()
{
  try
  {
    UserInput str_path = view_->get_input("Enter the path of the task to remove: ");
    model_.remove(parse_path(str_path));
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
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
  try
  {
    UserInput str_path =
        view_->get_input("Enter the path of the task to change the status of: ");
    UserInput status = view_->get_input(
        "Which status would you like to change it to (1-Not started, 2-In progress, "
        "3-Completed): ");

    model_.change_task_status(parse_path(str_path), std::stoul(status.text));
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}

void Controller::handle_prio_change()
{
  try
  {
    UserInput path_str =
        view_->get_input("Enter the path of the task to change the priority of: ");
    UserInput prio =
        view_->get_input("Enter the value you want to change it to (1-100): ");
    model_.change_task_prio(parse_path(path_str), std::stoul(prio.text));
  }
  catch (const std::out_of_range &e)
  {
    view_->display_msg("Error: Out of range access");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + std::string(e.what()));
  }
}
}  // namespace Todo
