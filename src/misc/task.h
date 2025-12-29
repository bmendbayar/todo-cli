#pragma once

#include <boost/describe.hpp>
#include <string>
#include <vector>

#include "status.h"

namespace Todo {
struct Task {
    std::string desc;
    std::vector<Task> child_tasks;
    int prio;
    Status status;
};
BOOST_DESCRIBE_STRUCT(Task, (), (desc, child_tasks, prio, status));
}  // namespace Todo
