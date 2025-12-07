#pragma once

#include <boost/describe.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace Todo
{
enum class Status : uint8_t
{
  NOT_STARTED = 1,
  IN_PROGRESS = 2,
  COMPLETED = 3
};
BOOST_DESCRIBE_ENUM(Status, NOT_STARTED, IN_PROGRESS, COMPLETED);

struct Task
{
  std::string desc;
  std::vector<Task> child_tasks;
  Status status;
};
BOOST_DESCRIBE_STRUCT(Task, (), (desc, child_tasks, status));

class Model
{
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
   */
  void add(const std::string &item_desc, const std::vector<size_t> &path);

  /**
   * @brief Remove @p index task from the todo list.
   *
   * @param index Index of task to remove.
   */
  void remove(const std::vector<size_t> &path);

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
   * @brief Toggles the completion status of @p index task.
   *
   * @param index Index of task to change.
   */
  void change_task_status(const std::vector<size_t> &path, int status);

  /**
   * @brief Returns a const reference of the todo list vector.
   */
  const std::vector<Task> &get_list();

private:
  std::vector<Task> todo_list_;  ///< Container to hold todo list tasks
                                 ///< while program is running.
};
}  // namespace Todo
