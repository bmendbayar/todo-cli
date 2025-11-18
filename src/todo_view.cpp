#include <iostream>
#include <vector>
#include <string>

#include "todo_view.h"

void TodoView::display_list(const std::vector<std::string> todo_list)
{
    int index = 1;
    for (const auto &str : todo_list)
    {
        std::cout << index << ". " << str << '\n';
        index++;
    }
}

void TodoView::display_msg(const std::string &msg)
{
    std::cout << msg << '\n';
}
