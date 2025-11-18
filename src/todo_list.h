#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <filesystem>

class TodoList
{
private:
    static inline std::filesystem::path TODO_DIR = ".todo";

    static inline std::filesystem::path TODOS = ".todo/todos.txt";

    std::vector<std::string> todo_list_;

public:
    TodoList();

    void init();

    void add(const std::string &todo_item);

    void remove(size_t index);

    void clear();

    void update_file();

    const std::vector<std::string> &get_list();
};
