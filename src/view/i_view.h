#pragma once

#include <ncurses.h>

#include "view.h"

namespace todo {
class IView : public View {
private:
    WINDOW *menu_win_;  ///< Curses window to display the menu.
    WINDOW
        *list_border_;  ///< Curses window to display a border around the list.
    WINDOW *list_pad_;  ///< Curses pad(scroll) to display the list.

    int scroll_offset_;  ///< Number of y scrolls.
    int list_height_;    ///< Height of the list pad.
    int list_width_;     ///< Width of the list pad.

public:
    /// \brief Default constructor.
    IView();

    /// \brief Destructor.
    ~IView();

    /// \brief Gets input from the user.
    /// \param msg String to display.
    virtual UserInput get_input(const std::string &msg) override;

    /// \brief Display task list to screen.
    /// \param todo_list Const reference to a vector.
    /// \param level Recursion depth.
    virtual void display_list(
        const std::vector<Task> &todo_list, u16 level = 0
    ) override;

    /// \brief Display message to screen.
    /// \param msg String to display.
    virtual void display_msg(const std::string &msg) override;

private:
    /// \brief Refreshes the list window (pad).
    void refresh_list_view();
};
}  // namespace todo
