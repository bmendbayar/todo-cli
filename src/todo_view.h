#pragma once

#include <vector>
#include <string>

#include "todo_list.h"

class TodoView
{
public:
    TodoView() = default;

    void display_list(const std::vector<std::string> todo_list);

    void display_msg(const std::string &msg);
};
