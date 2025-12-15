#include <boost/describe.hpp>
#include <boost/json.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "model.h"

namespace Todo
{
Model::Model()
{
  load_file();
}

void Model::load_file()
{
  dir_init();

  std::ifstream infile{TODO_DIR / TODO_FILE};
  if (infile.is_open() == false)
  {
    std::cerr << "error: file not opened\n";
    std::exit(EXIT_FAILURE);
  }

  size_t size = std::filesystem::file_size(TODO_DIR / TODO_FILE);
  std::string buf(size, ' ');
  infile.read(&buf[0], size);
  infile.close();

  try
  {
    boost::json::value jv = boost::json::parse(buf);
    if (jv.is_array() == false)
    {
      std::cerr << "json parsing error: json was not an array\n";
      std::exit(EXIT_FAILURE);
    }

    todo_list_ = boost::json::value_to<std::vector<Task>>(jv);
  }
  catch (const std::exception &e)
  {
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
  try
  {
    if (std::filesystem::exists(TODO_DIR) == false)
    {
      std::filesystem::create_directory(TODO_DIR);
    }

    if (std::filesystem::exists(TODO_DIR / TODO_FILE) == false)
    {
      std::ofstream outfile{TODO_DIR / TODO_FILE};
      outfile << "[]";

      if (outfile.is_open() == false)
      {
        std::cerr << "file could not be initialized\n";
      }
      outfile.close();
    }
  }
  catch (const std::exception &e)
  {
    return;
  }
}

void Model::add(const std::string &task_desc, const int prio,
                const std::vector<size_t> &path)
{
  if (path.empty())
  {
    todo_list_.emplace_back(Task{task_desc, {}, prio, Status::NOT_STARTED});
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end(); ++it)
  {
    curr = &(curr->child_tasks.at(*it));
  }

  curr->child_tasks.emplace_back(Task{task_desc, {}, prio, Status::NOT_STARTED});
  return;
}

void Model::remove(const std::vector<size_t> &path)
{
  if (path.empty())
  {
    return;
  }

  if (path.size() == 1)
  {
    todo_list_.erase(todo_list_.begin() + path[0]);
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end() - 1; ++it)
  {
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
  if (task.child_tasks.empty() == true)
  {
    return;
  }

  for (auto &t : task.child_tasks)
  {
    t.status = status;
    if (t.child_tasks.empty() == false)
    {
      change_child_task_status(t, status);
    }
  }
}

void Model::change_task_status(const std::vector<size_t> &path, const int status)
{
  if (path.empty())
  {
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end(); ++it)
  {
    if (status == 2)
    {
      curr->status = Status::IN_PROGRESS;
    }

    curr = &(curr->child_tasks.at(*it));
  }
  curr->status = static_cast<Status>(status);

  if (curr->child_tasks.empty() == false)
  {
    change_child_task_status(*curr, static_cast<Status>(status));
  }

  return;
}

void Model::change_task_prio(const std::vector<size_t> &path, const int prio)
{
  if (path.empty())
  {
    return;
  }

  Task *curr = &(todo_list_.at(path[0]));
  for (auto it = path.begin() + 1; it < path.end(); ++it)
  {
    curr = &(curr->child_tasks.at(*it));
  }
  curr->prio = prio;

  return;
}

const std::vector<Task> &Model::get_list()
{
  return todo_list_;
}
}  // namespace Todo
