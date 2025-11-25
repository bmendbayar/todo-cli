#include <curses.h>

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "basic_view.h"

void clear_input_buf()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Todo::MenuOptions Todo::BasicView::get_menu_opt()
{
  std::cout << "1. Add a task\n"
            << "2. Remove a task\n"
            << "3. Display todo list\n"
            << "4. Change completion status of a task\n"
            << "0. Exit\n"
            << "Enter choice: ";

  size_t opt;
  std::cin >> opt;
  clear_input_buf();

  if (opt >= 0 && opt <= 5)
  {
    return static_cast<MenuOptions>(opt);
  }
  return MenuOptions::INVALID;
}

std::string Todo::BasicView::get_task_desc(const std::string &msg)
{
  std::cout << msg;
  std::string desc;
  std::getline(std::cin, desc);
  return desc;
}

size_t Todo::BasicView::get_index(const std::string &msg)
{
  std::cout << msg;
  size_t index;
  std::cin >> index;
  clear_input_buf();
  return index;
}

void Todo::BasicView::display_list(const std::vector<Todo::Task> &todo_list,
                                   size_t level)
{
  size_t id = 1;
  for (const auto &task : todo_list)
  {
    std::cout << std::string(level, ' ') << id << ". " << task.desc << ' '
              << (task.completion ? "(done)" : "(in progress)") << '\n';

    if (todo_list[id - 1].child_tasks.size() != 0)
    {
      display_list(todo_list[id - 1].child_tasks, level + 1);
    }

    ++id;
  }
  std::cout << '\n';
}

void Todo::BasicView::display_msg(const std::string &msg)
{
  std::cout << msg << '\n';
}
