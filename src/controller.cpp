#include <cstdlib>
#include <exception>
#include <string>

#include "controller.h"
#include "basic_view.h"

Todo::Controller::Controller()
  : view_(new BasicView())
{
}

void Todo::Controller::run()
{
  bool running = true;
  while (running)
  {
    MenuOptions opt = view_->get_menu_opt();
    switch (opt)
    {
      case MenuOptions::ADD:
        handle_add();
        break;

      case MenuOptions::REMOVE:
        handle_remove();
        break;

      case MenuOptions::DISPLAY:
        handle_display();
        break;

      case MenuOptions::CHANGE_STATUS:
        handle_status_change();
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

void Todo::Controller::handle_add()
{
  std::string desc =
      view_->get_task_desc("Enter the description of your task: ");
  size_t index = view_->get_index("Enter parent ID (0 for root task): ");

  try
  {
    model_.add(desc, index);
    view_->display_msg("Successfully added.");
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + static_cast<std::string>(e.what()));
  }
}

void Todo::Controller::handle_remove()
{
  try
  {
    model_.remove(
        view_->get_index("Enter the index of the task you want to remove: "));
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + static_cast<std::string>(e.what()));
  }
}

void Todo::Controller::handle_display()
{
  view_->display_msg("\nCurrent list:");
  view_->display_list(model_.get_list());
}

void Todo::Controller::handle_status_change()
{
  try
  {
    model_.change_task_status(view_->get_index(
        "Enter the index of the task you want to change the status of: "));
  }
  catch (const std::exception &e)
  {
    view_->display_msg("Error: " + static_cast<std::string>(e.what()));
  }
}
