#pragma once

#include <cstdlib>
#include <filesystem>
#include <vector>

#include "task.h"
#include "types.h"

namespace todo {
class Model {
private:
  const static inline std::filesystem::path TODO_DIR = ".todo";            ///< Directory name.
  const static inline std::filesystem::path TODO_FILE = "todo_list.json";  ///< File name.

public:
  /// \brief Constructs an object by reading from a JSON file.
  Model();

  /// \brief Sets up the .todo directory and its contents.
  void dir_init();

  /// /brief Adds task with description to todo list.
  /// /param task The task to add.
  /// /param path Path of task to add.
  void add(Task task, const std::vector<u64> &path);

  /// /brief Remove task from the todo list.
  /// /param path Path of the task to remove.
  void remove(const std::vector<u64> &path);

  /// \brief Clears (empties) the list.
  void clear();

  /// \brief Load from a JSON file to populate todo list vector.
  /// \note Uses boost.JSON to parse file.
  void load_file();

  /// \brief Save todo list vector to a JSON file.
  /// \note Uses boost.JSON to serialize data.
  void save_file();

  /// \brief Changes the completion status of a task.
  /// \param path Path of the task to change.
  /// \param status Status to change to.
  void change_task_status(const std::vector<u64> &path, const Status status);

  /// \brief Changes the completion status of a task.
  /// \param path Path of the task to change.
  /// \param prio Priority to change to.
  void change_task_priority(const std::vector<u64> &path, const int priority);

  /// \brief Returns a const reference of the todo list vector.
  const std::vector<Task> &get_list();

private:
  std::vector<Task> todo_list_;  ///< Container to hold todo list tasks
                                 ///< while program is running.

  /// \brief Changes all child tasks' status according to parent.
  void change_child_task_status(Task &task, const Status status);
};
}  // namespace todo
