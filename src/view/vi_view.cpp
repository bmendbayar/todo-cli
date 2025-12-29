#include <cctype>
#include <string>

#include "vi_view.h"

namespace Todo {
constexpr std::string SEN = "deadbeef";

ViView::ViView()
{
    initscr();
    cbreak();
    noecho();
    curs_set(1);

    notif_ = newwin(2, 30, 0, COLS - 30);
    list_pad_ = newpad(1000, COLS);
    keypad(list_pad_, true);
    set_escdelay(25);
}

ViView::~ViView()
{
    delwin(notif_);
    delwin(list_pad_);
    endwin();
}

void ViView::refresh_list_view()
{
    prefresh(list_pad_, border_y_, border_x_, 0, 0, LINES - 1, COLS);
}

UserInput ViView::get_input(const std::string &msg)
{
    switch (mode_) {
        case Mode::NORMAL:
            return handle_normal();
        case Mode::REMOVE:
            mode_ = Mode::NORMAL;
            return handle_remove();
        case Mode::CHANGE:
            return handle_change();
        case Mode::SINSERT:
            return handle_insert();
        case Mode::CINSERT:
            return handle_insert();
        default:
            break;
    }

    return {{}, true};
}

UserInput ViView::handle_normal()
{
    int ch;
    while (true) {
        ch = wgetch(list_pad_);
        wclear(notif_);

        switch (ch) {
            case 'h':
                if (cursor_.x > 0) {
                    cursor_.x--;
                }
                break;
            case 'j':
                if (cursor_.y < 1000) {
                    cursor_.y++;
                }
                break;
            case 'k':
                if (cursor_.y > 0) {
                    cursor_.y--;
                }
                break;
            case 'l':
                if (cursor_.x < COLS) {
                    cursor_.x++;
                }
                break;

            case 'O':
                mode_ = Mode::SINSERT;
                return {std::string(1, ch), true};

            case 'o':
                mode_ = Mode::CINSERT;
                return {std::string(1, ch), true};

            case 'd':
                mode_ = Mode::REMOVE;
                return {std::string(1, ch), true};

            case 'c':
                mode_ = Mode::CHANGE;
                return {std::string(1, ch), true};

            case 'x':
                mode_ = Mode::CHANGE;
                return {std::string(1, ch), true};

            case 'q':
                return {std::string(1, ch), true};

            default:
                break;
        }

        if (cursor_.y == getmaxy(stdscr) + scroll_offset_ - 7) {
            scroll_offset_++;
            border_y_++;
        } else if (cursor_.y == scroll_offset_ + 6) {
            scroll_offset_--;
            border_y_--;
        }

        wmove(list_pad_, cursor_.y, cursor_.x);
        refresh_list_view();
    }
}

UserInput ViView::handle_insert()
{
    std::string buffer;
    int ch;

    if (ichain_ == InsertChain::PRIO) {
        ichain_ = InsertChain::DESC;
        mode_ = Mode::NORMAL;
        return {std::string(1, wgetch(list_pad_)), true};
    }

    if (ichain_ == InsertChain::PATH) {
        ichain_ = InsertChain::PRIO;
        if (mode_ == Mode::CINSERT) {
            return {std::to_string(cursor_.y), true};
        } else {
            return {std::to_string(cursor_.y + 1), true};
        }
    }

    if (mode_ == Mode::CINSERT && ichain_ == InsertChain::DESC) {
        handle_cinsert();
    } else if (mode_ == Mode::SINSERT && ichain_ == InsertChain::DESC) {
        handle_sinsert();
    }
    ichain_ = InsertChain::PATH;

    while (true) {
        mvwprintw(list_pad_, cursor_.y, 0, "%s",
                  std::string(buffer.length() + 5, ' ').c_str());
        mvwprintw(list_pad_, cursor_.y, 0, "%s", buffer.c_str());
        refresh_list_view();

        ch = wgetch(list_pad_);
        if (ch == '\n' || ch == KEY_ENTER) {
            break;
        } else if (ch == 27) {
            mode_ = Mode::NORMAL;
            return {SEN, true};
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (!buffer.empty()) {
                buffer.pop_back();
                --cursor_.x;
            }
        } else if (isprint(ch)) {
            buffer.push_back(ch);
            ++cursor_.x;
        }
    }

    cursor_.x -= buffer.size();
    wclear(notif_);
    return {buffer, true};
}

void ViView::handle_sinsert()
{
    winsertln(list_pad_);
    refresh_list_view();
}

void ViView::handle_cinsert()
{
    ++cursor_.y;
    wmove(list_pad_, cursor_.y, cursor_.x);
    winsertln(list_pad_);
    refresh_list_view();
}

UserInput ViView::handle_remove()
{
    int ch = wgetch(list_pad_);
    if (ch == 27) {
        return {SEN, true};
    }

    wclear(notif_);
    return {std::to_string(cursor_.y + 1), true};
}

UserInput ViView::handle_change()
{
    int ch = wgetch(list_pad_);
    if (ch == 27) {
        mode_ = Mode::NORMAL;
    } else if (isdigit(ch)) {
        mode_ = Mode::NORMAL;
        return {std::string(1, ch), true};
    } else if (ch == 'c' || ch == 'x') {
        return {std::to_string(cursor_.y + 1), true};
    }

    wclear(notif_);
    return {SEN, true};
}

void ViView::display_list(const std::vector<Task> &todo_list, u16 level)
{
    if (level == 0) {
        wclear(list_pad_);
    }

    u16 lsize = todo_list.size();
    for (u16 i = 0; i < lsize; ++i) {
        const auto &t = todo_list[i];
        std::string status;
        switch (t.status) {
            case Status::IN_PROGRESS:
                status = "-";
                break;
            case Status::COMPLETED:
                status = "X";
                break;
            case Status::NOT_STARTED:
            default:
                status = " ";
        }

        int y = getcury(list_pad_);
        int x = 1 + (level * 2);

        std::string prio;
        switch (t.prio) {
            case 1:
                prio = "Low";
                break;
            case 2:
                prio = "Medium";
                break;
            case 3:
                prio = "High";
                break;
            default:
                prio = "";
        }

        mvwprintw(list_pad_, y, x, "%zu. [%s] %s [%s]\n", i + 1, status.c_str(),
                  t.desc.c_str(), prio.c_str());

        if (!t.child_tasks.empty()) {
            display_list(t.child_tasks, level + 1);
        }
    }

    if (level == 0) {
        wmove(list_pad_, cursor_.y, cursor_.x);
        refresh_list_view();
    }
}

void ViView::display_msg(const std::string &msg)
{
    mvwprintw(notif_, 0, 0, "%s", msg.c_str());
    mvwprintw(notif_, 1, 0, "Press any key to continue. ");
    wrefresh(notif_);
}
}  // namespace Todo
