#pragma once

#include <ncurses.h>

#include "types.h"
#include "view.h"

namespace Todo {
class ViView : public View {
private:
    WINDOW *list_pad_{nullptr};
    WINDOW *notif_{nullptr};

    struct {
        u16 x{};
        u16 y{};
    } cursor_;

    int border_y_{};
    int border_x_{};
    int scroll_offset_{};

    enum class Mode : u8 {
        NORMAL = 0,
        REMOVE = 1,
        CHANGE = 2,
        SINSERT = 3,
        CINSERT = 4,
    } mode_{Mode::NORMAL};

    enum class InsertChain : u8 {
        DESC = 0,
        PATH = 1,
        PRIO = 2
    } ichain_{InsertChain::DESC};

public:
    /// \brief Default constructor.
    ViView();

    /// \brief Destructor.
    ~ViView();

    /// \brief Gets input from the user.
    /// \param msg String to display.
    virtual UserInput get_input(const std::string &msg) override;

    /// \brief Display @p task list to screen.
    /// \param todo_list Const reference to a vector.
    /// \param level Recursion depth.
    virtual void display_list(const std::vector<Task> &todo_list, u16 level = 0) override;

    /// \brief Display @p message to screen.
    /// \param msg String to display.
    virtual void display_msg(const std::string &msg) override;

private:
    void refresh_list_view();

    UserInput handle_normal();
    UserInput handle_remove();
    UserInput handle_change();

    UserInput handle_insert();
    void handle_cinsert();
    void handle_sinsert();
};
}  // namespace Todo
