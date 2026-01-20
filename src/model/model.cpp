#include <boost/json.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "model.h"
#include "task.h"

namespace todo {
void Model::load_file()
{
    dir_init();

    std::ifstream infile{TODO_DIR / TODO_FILE};
    if (infile.is_open() == false) {
        throw std::runtime_error("file was not opened");
    }

    u64 size = std::filesystem::file_size(TODO_DIR / TODO_FILE);
    std::string buf(size, ' ');
    infile.read(&buf[0], size);
    infile.close();

    boost::json::value jv = boost::json::parse(buf);
    todo_list_ = boost::json::value_to<std::vector<Task>>(jv);
}

void Model::save_file()
{
    std::ofstream outfile{TODO_DIR / TODO_FILE};
    boost::json::value jv = boost::json::value_from(todo_list_);
    outfile << jv;
    outfile.close();
}

void Model::dir_init()
{
    if (std::filesystem::exists(TODO_DIR) == false) {
        std::filesystem::create_directory(TODO_DIR);
    }

    if (std::filesystem::exists(TODO_DIR / TODO_FILE) == false) {
        std::ofstream outfile{TODO_DIR / TODO_FILE};
        outfile << "[]";

        if (outfile.is_open() == false) {
            std::cerr << "file could not be initialized\n";
        }
        outfile.close();
    }
}

Task *Model::get_parent_task(const std::vector<u64> &path)
{
    if (path.empty() == true) {
        return nullptr;
    }

    if (path.size() <= 1) {
        return nullptr;
    }

    if (todo_list_.size() < path[0]) {
        return nullptr;
    }
    Task *curr = &(todo_list_[path[0]]);

    for (auto it = path.begin() + 1; it < path.cend() - 1; ++it) {
        if (todo_list_.size() < *it) {
            return nullptr;
        }
        curr = &(curr->child_tasks.at(*it));
    }

    return curr;
}

Task *Model::get_task(const std::vector<u64> &path)
{
    if (path.empty() == true) {
        return nullptr;
    }

    auto task = get_parent_task(path);
    if (task == nullptr) {
        if (todo_list_.size() < *(path.cend() - 1)) {
            return nullptr;
        }
        return &todo_list_[*(path.cend() - 1)];
    }

    if (task->child_tasks.size() < *(path.cend() - 1)) {
        return nullptr;
    }

    return &(task->child_tasks[*(path.cend() - 1)]);
}

void Model::add(Task &task, const std::vector<u64> &path)
{
    Task *parent_task = get_parent_task(path);
    if (parent_task == nullptr) {
        todo_list_.emplace(todo_list_.cbegin() + *(path.cend() - 1), task);
        return;
    }

    parent_task->child_tasks.emplace(
        parent_task->child_tasks.cbegin() + *(path.cend() - 1), task
    );
    return;
}

void Model::remove(const std::vector<u64> &path)
{
    if (path.size() == 1) {
        todo_list_.erase(todo_list_.begin() + path[0]);
        return;
    }

    if (path.empty()) {
        throw std::runtime_error("rm err: path was empty");
    }

    Task *parent_task = get_parent_task(path);
    if (parent_task == nullptr) {
        throw std::out_of_range("rm err: parent task was null");
    }
    parent_task->child_tasks.erase(
        parent_task->child_tasks.begin() + *(path.end() - 1)
    );

    return;
}

void Model::clear()
{
    todo_list_.clear();
}

void Model::change_child_task_status(Task &task, const Status status)
{
    if (task.child_tasks.empty() == true) {
        return;
    }

    for (auto &t : task.child_tasks) {
        t.status = status;
        if (t.child_tasks.empty() == false) {
            change_child_task_status(t, status);
        }
    }
}

void Model::change_task_status(
    const std::vector<u64> &path, const Status status
)
{
    if (path.empty()) {
        throw std::runtime_error("change status err: path was empty");
    }

    // necessary duplicate code for task inheritance.
    Task *curr = &(todo_list_.at(path[0]));
    for (auto it = path.begin() + 1; it < path.end(); ++it) {
        if (status == Status::IN_PROGRESS) {
            curr->status = Status::IN_PROGRESS;
        }
        curr = &(curr->child_tasks.at(*it));
    }
    curr->status = status;

    if (curr->child_tasks.empty() == true) {
        return;
    }

    if (curr->status == Status::COMPLETED) {
        change_child_task_status(*curr, status);
    }

    return;
}

void Model::change_task_priority(
    const std::vector<u64> &path, const u16 priority
)
{
    if (path.empty()) {
        throw std::runtime_error("change priority err: path was empty");
    }

    Task *task = get_task(path);
    task->priority = priority;
    return;
}

const std::vector<Task> &Model::get_list()
{
    return todo_list_;
}
}  // namespace todo
