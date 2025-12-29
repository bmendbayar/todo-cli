#pragma once

#include <ncurses.h>

#include "view.h"

namespace Todo {
class IView : public View {
private:
    WINDOW *menu_win_;
    WINDOW *list_win_;
    WINDOW *list_pad_;

    int scroll_offset_;
    int list_height_;
    int list_width_;

public:
    /**
     * @brief Default constructor.
     */
    IView();

    /**
     * @brief Destructor.
     */
    ~IView();

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

private:
    /**
     * @brief Refreshes the list window (pad).
     */
    void refresh_list_view();
};
}  // namespace Todo
