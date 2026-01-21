#include <cctype>
#include <chrono>
#include <stdexcept>
#include <string>

#include "vi_view.h"

namespace todo {
ViView::ViView()
{
    initscr();
    if (has_colors() == false) {
        throw std::runtime_error("terminal has no color support");
    }
    start_color();
    cbreak();
    noecho();
    curs_set(1);

    notif_ = newwin(2, COLS, LINES - 2, 0);
    wattron(notif_, A_REVERSE);
    wrefresh(notif_);
    list_pad_ = newpad(1000, COLS);
    keypad(list_pad_, true);
    set_escdelay(25);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);   // low
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // medium
    init_pair(3, COLOR_RED, COLOR_BLACK);     // high
    init_pair(4, COLOR_WHITE, COLOR_BLACK);   // regular text
}

ViView::~ViView()
{
    delwin(notif_);
    delwin(list_pad_);
    endwin();
}

void ViView::refresh_list_view()
{
    prefresh(list_pad_, border_y_, border_x_, 0, 0, LINES - 3, COLS);
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
        case Mode::SIBLING_INSERT:
            return handle_insert();
        case Mode::CHILD_INSERT:
            return handle_insert();
        case Mode::DESC_CHANGE:
            return handle_insert();
        default:
            break;
    }

    return {{}, true, false};
}

UserInput ViView::handle_normal()
{
    int ch;
    while (true) {
        ch = wgetch(list_pad_);

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

            case 'g':
                return {std::string(1, ch), true, false};

            case 'u':
                return {std::string(1, ch), true, false};

            case 'O':
                mode_ = Mode::SIBLING_INSERT;
                return {std::string(1, ch), true, false};

            case 'o':
                mode_ = Mode::CHILD_INSERT;
                return {std::string(1, ch), true, false};

            case 'd':
                mode_ = Mode::REMOVE;
                return {std::string(1, ch), true, false};

            case 'v':
                mode_ = Mode::DESC_CHANGE;
                return {std::string(1, ch), true, false};

            case 'c':
                mode_ = Mode::CHANGE;
                return {std::string(1, ch), true, false};

            case 'x':
                mode_ = Mode::CHANGE;
                return {std::string(1, ch), true, false};

            case 'q':
                return {std::string(1, ch), true, false};

            default:
                break;
        }

        if (cursor_.y == getmaxy(stdscr) + scroll_offset_ - 9) {
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

inline i32 find_indent_lvl(WINDOW *win, int cursor_y)
{
    i32 indent{};
    for (i32 i = 0; i < COLS; ++i) {
        if ((mvwinch(win, cursor_y, i) & A_CHARTEXT) == '[') {
            indent = i;
            break;
        }
    }
    return indent;
}

UserInput ViView::handle_insert()
{
    std::string buf;
    int ch;

    if (curr_event_ == InsertChain::DESC) {
        initial_cursor_x_ = cursor_.x;
    }

    if (curr_event_ == InsertChain::PATH) {
        curr_event_ = InsertChain::PRIORITY;
        switch (mode_) {
            case Mode::CHILD_INSERT:
                return {std::to_string(cursor_.y), true, false};
            case Mode::DESC_CHANGE:
                mode_ = Mode::NORMAL;
                curr_event_ = InsertChain::DESC;
            case Mode::SIBLING_INSERT:
            default:
                return {std::to_string(cursor_.y + 1), true, false};
        }
    }

    if (mode_ == Mode::CHILD_INSERT && curr_event_ == InsertChain::DESC) {
        handle_child_insert();
        initial_cursor_x_ = cursor_.x;
        curr_event_ = InsertChain::PATH;
    } else if (mode_ == Mode::SIBLING_INSERT &&
               curr_event_ == InsertChain::DESC) {
        handle_sibling_insert();
        initial_cursor_x_ = cursor_.x;
        curr_event_ = InsertChain::PATH;
    } else if (mode_ == Mode::DESC_CHANGE && curr_event_ == InsertChain::DESC) {
        // find text starting point and erase
        cursor_.x = find_indent_lvl(list_pad_, cursor_.y) + 2;
        initial_cursor_x_ = cursor_.x;
        curr_event_ = InsertChain::PATH;
        for (u16 i{cursor_.x}; i < COLS; ++i) {
            mvwprintw(list_pad_, cursor_.y, i, " ");
        }
    }

    wattron(list_pad_, COLOR_PAIR(4));

    i16 buf_size{};
    if (curr_event_ == InsertChain::DATE) {
        buf_size = 10;
    } else if (curr_event_ == InsertChain::PRIORITY) {
        buf_size = 3;
    } else {
        buf_size = COLS;
    }
    buf.reserve(buf_size);

    i16 padding = cursor_.x;
    for (i16 i{}; i < buf_size; ++i) {
        if (curr_event_ != InsertChain::PRIORITY) {
            mvwprintw(
                list_pad_,
                cursor_.y,
                padding + 1,
                "%s",
                std::string(buf.length() + 5, ' ').c_str()
            );
            mvwprintw(list_pad_, cursor_.y, padding + 1, "%s", buf.c_str());
            refresh_list_view();
        }

        ch = wgetch(list_pad_);
        if (ch == '\n' || ch == KEY_ENTER) {
            break;
        } else if (ch == 27) {  // 27 == esc
            curr_event_ = InsertChain::DESC;
            mode_ = Mode::NORMAL;
            return {{}, true, true};
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
            if (!buf.empty()) {
                buf.pop_back();
                --cursor_.x;
                i -= 2;
            }
        } else if (isprint(ch)) {
            buf.push_back(ch);
            ++cursor_.x;
        }
    }

    if (curr_event_ == InsertChain::DATE) {
        curr_event_ = InsertChain::DESC;
        cursor_.x = initial_cursor_x_;
        mode_ = Mode::NORMAL;
    }

    if (curr_event_ == InsertChain::PRIORITY) {
        curr_event_ = InsertChain::DATE;
    }

    return {buf, true, false};
}

void ViView::handle_sibling_insert()
{
    cursor_.x = find_indent_lvl(list_pad_, cursor_.y);
    wmove(list_pad_, cursor_.y, cursor_.x);
    winsertln(list_pad_);
    refresh_list_view();
}

void ViView::handle_child_insert()
{
    cursor_.x = find_indent_lvl(list_pad_, cursor_.y) + 2;
    ++cursor_.y;
    wmove(list_pad_, cursor_.y, cursor_.x);
    winsertln(list_pad_);
    refresh_list_view();
}

UserInput ViView::handle_remove()
{
    int ch = wgetch(list_pad_);
    if (ch != 'd') {
        return {{}, true, false};
    }

    return {std::to_string(cursor_.y + 1), true, false};
}

UserInput ViView::handle_change()
{
    int ch = wgetch(list_pad_);
    if (ch == 27) {
        mode_ = Mode::NORMAL;
    } else if (isdigit(ch)) {
        mode_ = Mode::NORMAL;
        return {std::string(1, ch), true, false};
    } else if (ch == 'c' || ch == 'x') {
        return {std::to_string(cursor_.y + 1), true, false};
    }

    return {{}, true, false};
}

void ViView::display_list(const std::vector<Task> &todo_list, u16 level)
{
    if (level == 0) {
        wclear(list_pad_);
    }

    std::chrono::time_point<std::chrono::system_clock> now{};
    std::chrono::year_month_day today{};
    if (level == 0) {
        now = std::chrono::system_clock::now();
        std::chrono::year_month_day today{
            std::chrono::floor<std::chrono::days>(now)
        };
    }

    u16 lsize = todo_list.size();
    for (u16 i = 0; i < lsize; ++i) {
        const auto &task = todo_list[i];
        // omit completed tasks
        if (task.status == Status::COMPLETED) {
            continue;
        }

        int y = getcury(list_pad_);
        int x = 1 + (level * 2);

        if (task.priority < 30) {
            wattron(list_pad_, COLOR_PAIR(1));
        } else if (task.priority < 70) {
            wattron(list_pad_, COLOR_PAIR(2));
        } else {
            wattron(list_pad_, COLOR_PAIR(3));
        }

        std::string status;
        switch (task.status) {
            case Status::NOT_STARTED:
                status = " ";
                break;
            case Status::IN_PROGRESS:
                status = "~";
                break;
            case Status::COMPLETED:
                status = "X";
                break;
            default:
                break;
        }

        std::string overdue = [&task, &today]() -> std::string {
            if ((int)today.year() > task.due_date.year) {
                return "OVERDUE";
            }

            if ((int)today.year() == task.due_date.year &&
                (unsigned)today.month() > task.due_date.month) {
                return "OVERDUE";
            }

            if ((int)today.year() == task.due_date.year &&
                (unsigned)today.month() == task.due_date.month &&
                (unsigned)today.day() > task.due_date.day) {
                return "OVERDUE";
            }

            return "";
        }();

        mvwprintw(
            list_pad_,
            y,
            x,
            "[%s] %s %s (%d/%d/%d)\n",
            status.c_str(),
            task.desc.c_str(),
            overdue.c_str(),
            task.due_date.month,
            task.due_date.day,
            task.due_date.year
        );

        if (!task.child_tasks.empty()) {
            display_list(task.child_tasks, level + 1);
        }
    }

    if (level == 0) {
        wmove(list_pad_, cursor_.y, cursor_.x);
        refresh_list_view();
    }
}

void ViView::display_msg(const std::string &msg)
{
    mvwprintw(notif_, 0, 1, "%s", msg.c_str());
    mvwprintw(notif_, 1, 1, "Press any key to continue. ");
    wgetch(notif_);
    wrefresh(notif_);
}
}  // namespace todo
