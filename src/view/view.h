#pragma once

#include <string>
#include <vector>

#include "task.h"
#include "types.h"
#include "user_input.h"

namespace Todo {
class View {
public:
    /**
     * @brief Default Constructor.
     */
    View() = default;

    /**
     * @brief Destructor
     */
    virtual ~View() = default;

    /**
     * @brief Gets input from the user.
     *
     * @param msg String to display.
     */
    virtual UserInput get_input(const std::string &msg) = 0;

    /**
     * @brief Display @p task list to screen.
     *
     * @param todo_list Const reference to a vector.
     * @param level Recursion depth.
     */
    virtual void display_list(const std::vector<Task> &todo_list, u16 level = 0) = 0;

    /**
     * @brief Display @p message to screen.
     *
     * @param msg String to display.
     */
    virtual void display_msg(const std::string &msg) = 0;
};
}  // namespace Todo
