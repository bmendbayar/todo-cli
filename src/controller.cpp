#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#include "controller.h"

void Todo::Controller::run()
{
  constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

  bool exit = false;
  while (exit == false)
  {
    int x = get_input();
    std::cin.ignore(max_size, '\n');

    switch (x)
    {
      case 1:
      {
        std::cout << "enter todo item to add: ";
        std::string todo;
        std::getline(std::cin, todo);
        std::cout << '\n';
        pass_add(todo);
        break;
      }

      case 2:
      {
        int index;
        std::cout << "enter todo item to remove: ";
        std::cin >> index;
        pass_rm(index);
        break;
      }

      case 3:
      {
        std::cout << '\n';
        pass_display();
        std::cout << '\n';
        break;
      }

      case 0:
      {
        exit = true;
        break;
      }
    }
  }
}

int Todo::Controller::get_input()
{
  int x;
  std::cout << "1. add\n";
  std::cout << "2. remove\n";
  std::cout << "3. show\n";
  std::cout << "0. exit\n";
  std::cout << "choose: ";
  std::cin >> x;
  return x;
}

void Todo::Controller::pass_add(const std::string &task_desc)
{
  model_.add(task_desc);
}

void Todo::Controller::pass_rm(size_t index)
{
  model_.remove(index);
}

void Todo::Controller::pass_display()
{
  view_.display_list(model_.get_list());
}
