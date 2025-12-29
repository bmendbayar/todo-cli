#pragma once

#include <boost/describe.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

#include "task.h"
#include "types.h"

namespace Todo {
class Model {
private:
    const static inline std::filesystem::path TODO_DIR = ".todo";
    const static inline std::filesystem::path TODO_FILE = "todo_list.json";

public:
    /**
     * @brief Constructs an object by reading from a JSON file.
     */
    Model();

    /**
     * @brief Sets up the .todo directory and its contents.
     */
    void dir_init();

    /**
     * @brief Adds task with description @p task_desc to todo list.
     *
     * @param task_desc Description of task to add.
     * @param prio Priority of task to add.
     * @param path Path of task to add.
     */
    void add(const std::string &item_desc, const int prio, const std::vector<u16> &path);

    /**
     * @brief Remove @p index task from the todo list.
     *
     * @param path Path of the task to remove.
     */
    void remove(const std::vector<u16> &path);

    /**
     * @brief Clears (empties) the list.
     */
    void clear();

    /**
     * @brief Load from a JSON file to populate todo list vector.
     * @note Uses boost.JSON to parse file.
     */
    void load_file();

    /**
     * @brief Save todo list vector to a JSON file.
     * @note Uses boost.JSON to serialize data.
     */
    void save_file();

    /**
     * @brief Changes the completion status of @p index task.
     *
     * @param path Path of the task to change.
     * @param status Status to change to.
     */
    void change_task_status(const std::vector<u16> &path, const int status);

    /**
     * @brief Changes the completion status of @p index task.
     *
     * @param path Path of the task to change.
     * @param prio Priority to change to.
     */
    void change_task_prio(const std::vector<u16> &path, const int prio);

    /**
     * @brief Returns a const reference of the todo list vector.
     */
    const std::vector<Task> &get_list();

private:
    std::vector<Task> todo_list_;  ///< Container to hold todo list tasks
                                   ///< while program is running.

    /**
     * @brief Changes all child tasks' status according to parent.
     */
    void change_child_task_status(Task &task, const Status status);
};
}  // namespace Todo
