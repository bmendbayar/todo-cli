#pragma once

#include <cstdlib>
#include <string>

#include "todo_list.h"
#include "todo_view.h"

class TodoController
{
private:
    TodoList model_;
    TodoView view_;

public:
    TodoController() = default;

    void run();

private:
    int get_input();

    void pass_add(const std::string &todo);

    void pass_rm(size_t index);

    void pass_display();
};
