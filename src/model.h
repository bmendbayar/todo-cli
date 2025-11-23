#pragma once

#include <boost/describe.hpp>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

namespace Todo
{
struct Task
{
  size_t id;
  std::string desc;
  bool completion;
};
BOOST_DESCRIBE_STRUCT(Task, (), (id, desc, completion));

class Model
{
private:
  static inline std::filesystem::path TODO_DIR = ".todo";
  static inline std::filesystem::path TODO_FILE = "todo_list.json";

public:
  /**
   * @brief Constructs an object by reading from a JSON file.
   */
  Model();

  /**
   * @brief Destructor.
   * @note Calls save_file().
   */
  ~Model();

  /**
   * @brief Sets up the .todo directory and its contents.
   */
  void dir_init();

  /**
   * @brief Adds task with description @p task_desc to todo list.
   *
   * @param task_desc Description of task to add.
   */
  void add(const std::string &item_desc);

  /**
   * @brief Remove @p index task from the todo list.
   *
   * @param index Index of task to remove.
   */
  void remove(size_t index);

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
   * @brief Returns a const reference of the todo list vector.
   */
  const std::vector<Task> &get_list();

private:
  std::vector<Task> todo_list_;  ///< Container to hold todo list tasks
                                 ///< while program is running.
};
}  // namespace Todo
