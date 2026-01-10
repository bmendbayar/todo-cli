#pragma once

#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <cstdint>
#include <string>
#include <vector>

namespace todo {
struct Task {
  std::string desc;               ///< Description of the task.
  std::vector<Task> child_tasks;  ///< Child tasks.
  uint16_t priority;              ///< Priority of the task.

  enum class Status : char {
    NOT_STARTED = 1,
    IN_PROGRESS = 2,
    COMPLETED = 3,
    INVALID = 4
  } status;  ///< Completion status of the task.

  struct Date {
    uint16_t year{};
    uint16_t month{};
    uint16_t day{};
  } due_date;  ///< Due date of the task.

  bool operator==(const Task &other) const
  {
    if (desc != other.desc) {
      return false;
    }

    if (priority != other.priority) {
      return false;
    }

    if (status != other.status) {
      return false;
    }

    if (child_tasks != other.child_tasks) {
      return false;
    }

    if (due_date.year != other.due_date.year) {
      return false;
    }

    if (due_date.month != other.due_date.month) {
      return false;
    }

    if (due_date.day != other.due_date.day) {
      return false;
    }

    return true;
  }

  bool operator!=(const Task &other) const
  {
    return not(*this == other);
  }
};

inline void tag_invoke(boost::json::value_from_tag, boost::json::value &v,
                       todo::Task::Status const &s)
{
  v = static_cast<int>(s);
}

inline todo::Task::Status tag_invoke(boost::json::value_to_tag<todo::Task::Status>,
                                     boost::json::value const &v)
{
  return static_cast<todo::Task::Status>(v.as_int64());
}

BOOST_DESCRIBE_ENUM(Task::Status, NOT_STARTED, IN_PROGRESS, COMPLETED);
BOOST_DESCRIBE_STRUCT(Task::Date, (), (year, month, day));
BOOST_DESCRIBE_STRUCT(Task, (), (desc, child_tasks, priority, status, due_date));
}  // namespace todo
