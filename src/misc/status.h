#pragma once

#include <boost/describe.hpp>

#include "types.h"

namespace Todo {
enum class Status : u8 {
    NOT_STARTED = 1,
    IN_PROGRESS = 2,
    COMPLETED = 3
};
BOOST_DESCRIBE_ENUM(Status, NOT_STARTED, IN_PROGRESS, COMPLETED);
}  // namespace Todo
