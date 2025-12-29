#pragma once

#include "task.h"
#include "view.h"

namespace Todo {
class BasicView : public View {
public:
    /**
     * @brief Default constructor.
     */
    BasicView() = default;

    /**
     * @brief Gets input from the user.
     *
     * @param msg String to display.
     */
    virtual UserInput get_input(const std::string &msg) override;

    /**
     * @brief Display @p task list to screen.
     *
     * @param todo_list Const reference to a vector.
     * @param level Recursion depth.
     */
    virtual void display_list(const std::vector<Task> &todo_list, u16 level = 0) override;

    /**
     * @brief Display @p message to screen.
     *
     * @param msg String to display.
     */
    virtual void display_msg(const std::string &msg) override;
};
}  // namespace Todo
