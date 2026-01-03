#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "model.h"
#include "task.h"

namespace Todo {
Model::Model()
{
  load_file();
}

void Model::load_file()
{
  dir_init();

  std::ifstream infile{TODO_DIR / TODO_FILE};
  if (infile.is_open() == false) {
    std::cerr << "error: file not opened\n";
    std::exit(EXIT_FAILURE);
  }

  u64 size = std::filesystem::file_size(TODO_DIR / TODO_FILE);
  std::string buf(size, ' ');
  infile.read(&buf[0], size);
  infile.close();

  try {
    boost::json::value jv = boost::json::parse(buf);
    if (jv.is_array() == false) {
      std::cerr << "json parsing error: json was not an array\n";
      std::exit(EXIT_FAILURE);
    }

    todo_list_ = boost::json::value_to<std::vector<Task>>(jv);
  } catch (const std::exception &e) {
    std::cerr << "json parsing error: " << e.what() << '\n';
    std::exit(EXIT_FAILURE);
  }
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
  try {
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
  } catch (const std::exception &e) {
    return;
  }
}

void Model::add(Task task, const std::vector<u16> &path)
{
  if (path.empty()) {
    todo_list_.emplace_back(std::move(task));
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end(); ++it) {
    curr = &(curr->child_tasks.at(*it));
  }

  curr->child_tasks.emplace_back(std::move(task));
  return;
}

void Model::remove(const std::vector<u16> &path)
{
  if (path.empty()) {
    return;
  }

  if (path.size() == 1) {
    todo_list_.erase(todo_list_.begin() + path[0]);
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end() - 1; ++it) {
    curr = &(curr->child_tasks.at(*it));
  }

  curr->child_tasks.erase(curr->child_tasks.begin() + *(path.end() - 1));
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

void Model::change_task_status(const std::vector<u16> &path, const Status status)
{
  if (path.empty()) {
    return;
  }

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

void Model::change_task_priority(const std::vector<u16> &path, const int priority)
{
  if (path.empty()) {
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end(); ++it) {
    curr = &(curr->child_tasks.at(*it));
  }
  curr->priority = priority;

  return;
}

const std::vector<Task> &Model::get_list()
{
  return todo_list_;
}
}  // namespace Todo
