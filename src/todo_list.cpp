#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "todo_list.h"

namespace fs = std::filesystem;

TodoList::TodoList()
{
    init();

    std::ifstream infile(TODOS);
    if (infile.is_open() == false)
    {
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(infile, line))
    {
        todo_list_.emplace_back(line);
    }

    infile.close();
}

void TodoList::init()
{
    try
    {
        if (fs::exists(TODO_DIR) == false)
        {
            fs::create_directory(TODO_DIR);
        }

        if (fs::exists(TODOS) == false)
        {
            std::ofstream{ TODOS };
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}

void TodoList::add(const std::string &todo_item)
{
    todo_list_.emplace_back(todo_item);
}

void TodoList::remove(size_t index)
{
    todo_list_.erase(todo_list_.begin() + index - 1);
}

void TodoList::clear()
{
    std::ofstream outfile(TODOS);
    outfile << "\0";
    outfile.close();
}

void TodoList::update_file()
{
    clear();

    std::ofstream outfile(TODOS, std::ios::app);
    for (const auto &todo_item : todo_list_)
    {
        outfile << todo_item << '\n';
    }
    outfile.close();
}

const std::vector<std::string> &TodoList::get_list()
{
    return todo_list_;
}
