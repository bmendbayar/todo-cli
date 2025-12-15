#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <string>

#include "basic_view.h"
#include "controller.h"
#include "i_view.h"
#include "view.h"

Todo::Controller::Controller(int argc, char **argv)
{
  if (argc == 1)
  {
    view_ = new IView();
  }
  else if (strcmp(argv[1], "-b") == 0)
  {
    view_ = new BasicView();
  }
  else
  {
    view_ = new IView();
  }
}

void Todo::Controller::run()
{
  bool running = true;
  while (running)
  {
    handle_display();
    std::string sopt = view_->get_input(
        "=== Todo Menu ===\n"
        "1. Add task\n"
        "2. Remove task\n"
        "3. Change task status\n"
        "4. Change task priority\n"
        "5. Clear list\n"
        "0. Exit\n");
    if (sopt.length() > 1)
    {
      continue;
    }

    MenuOptions opt = static_cast<MenuOptions>(std::stoul(sopt));
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

std::vector<size_t> Todo::Controller::parse_path(const std::string &spath)
{
  std::vector<size_t> path;
  path.reserve(spath.length());

  for (const char &c : spath)
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

  return path;
}

void Todo::Controller::handle_add()
{
  try
  {
    std::string desc = view_->get_input("Enter the description of your task: ");
    std::string spath = view_->get_input("Enter the path of the new task: ");
    std::string sprio = view_->get_input("Enter the priority of the task (1-100): ");
    model_.add(desc, std::stoul(sprio), parse_path(spath));
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

void Todo::Controller::handle_remove()
{
  try
  {
    std::string spath = view_->get_input("Enter the path of the task to remove: ");
    model_.remove(parse_path(spath));
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

void Todo::Controller::handle_display()
{
  view_->display_list(model_.get_list());
}

void Todo::Controller::handle_clear()
{
  model_.clear();
}

void Todo::Controller::handle_status_change()
{
  try
  {
    std::string spath =
        view_->get_input("Enter the path of the task to change the status of: ");
    std::string sstatus = view_->get_input(
        "Which status would you like to change it to (1-Not started, 2-In progress, "
        "3-Completed): ");

    model_.change_task_status(parse_path(spath), atoi(&sstatus[0]));
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

void Todo::Controller::handle_prio_change()
{
  try
  {
    std::string spath =
        view_->get_input("Enter the path of the task to change the priority of: ");
    std::string sprio =
        view_->get_input("Enter the value you want to change it to (1-100): ");
    model_.change_task_prio(parse_path(spath), std::stoul(sprio));
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
