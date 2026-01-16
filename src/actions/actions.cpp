#include "actions/actions.h"

namespace todo {
Action::Action(Model &model, std::vector<u16> path)
  : model_(&model)
  , exe_path_(std::move(path))
{
}

RemoveAction::RemoveAction(Model &model, std::vector<u16> path)
  : Action(model, path)
  , undo_path_(path)
{
  if (path.empty() == false) {
    undo_path_.pop_back();
    const auto &local_list = model_->get_list();
    const Task *curr_task = &local_list[exe_path_[0]];
    auto size = exe_path_.size();
    for (u64 i{1}; i < size; ++i) {
      curr_task = &curr_task->child_tasks[exe_path_[i]];
    }
    task_ = *curr_task;
  } else {
    task_ = {};
  }
}

void RemoveAction::execute()
{
  if (task_.desc.empty()) {
    return;
  }
  model_->remove(exe_path_);
}

void RemoveAction::undo()
{
  if (task_.desc.empty()) {
    return;
  }
  model_->add(task_, undo_path_);
}

AddAction::AddAction(Model &model, std::vector<u16> path, Task task)
  : Action(model, path)
  , undo_path_(path)
  , task_(task)
{
  if (path.empty() == false) {
    const auto &local_list = model_->get_list();
    const Task *curr_task = &local_list[exe_path_[0]];
    auto size = exe_path_.size();
    for (u64 i{1}; i < size; ++i) {
      curr_task = &curr_task->child_tasks[exe_path_[i]];
    }
    undo_path_.push_back(curr_task->child_tasks.size());
  } else {
    undo_path_ = {0};
  }
}

void AddAction::execute()
{
  model_->add(task_, exe_path_);
}

void AddAction::undo()
{
  model_->remove(undo_path_);
}

StatusChangeAction::StatusChangeAction(Model &model, std::vector<u16> path, Status new_status)
  : Action(model, path)
  , new_status_(new_status)
{
  if (path.empty() == false) {
    const auto &local_list = model_->get_list();
    const Task *curr_task = &local_list[exe_path_[0]];
    u64 size = exe_path_.size();
    for (u64 i{1}; i < size; ++i) {
      curr_task = &curr_task->child_tasks[exe_path_[i]];
    }
    old_status_ = curr_task->status;
  }
}

void StatusChangeAction::execute()
{
  model_->change_task_status(exe_path_, new_status_);
}

void StatusChangeAction::undo()
{
  model_->change_task_status(exe_path_, old_status_);
}

PriorityChangeAction::PriorityChangeAction(Model &model, std::vector<u16> path, u16 new_priority)
  : Action(model, path)
  , new_priority_(new_priority)
{
  if (path.empty() == false) {
    const auto &local_list = model_->get_list();
    const Task *curr_task = &local_list[exe_path_[0]];
    u64 size = exe_path_.size();
    for (u64 i{1}; i < size; ++i) {
      curr_task = &curr_task->child_tasks[exe_path_[i]];
    }
    old_priority_ = curr_task->priority;
  }
}

void PriorityChangeAction::execute()
{
  model_->change_task_priority(exe_path_, new_priority_);
}

void PriorityChangeAction::undo()
{
  model_->change_task_priority(exe_path_, old_priority_);
}
}  // namespace todo
