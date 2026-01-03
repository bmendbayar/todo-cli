#include "actions/actions.h"

namespace Todo {
Action::Action(Model &model, std::vector<u16> path)
  : model_(&model)
  , exe_path_(std::move(path))
{
}

RemoveAction::RemoveAction(Model &model, std::vector<u16> path)
  : Action(model, path)
  , undo_path_(path)
{
  undo_path_.pop_back();

  const auto &local_list = model_->get_list();
  const Task *curr_t = &local_list[exe_path_[0]];
  auto size = exe_path_.size();
  for (u64 i{1}; i < size; ++i) {
    curr_t = &curr_t->child_tasks[exe_path_[i]];
  }
  task_ = *curr_t;
}

void RemoveAction::execute()
{
  model_->remove(exe_path_);
}

void RemoveAction::undo()
{
  model_->add(task_, undo_path_);
}

AddAction::AddAction(Model &model, std::vector<u16> path, Task task)
  : Action(model, path)
  , undo_path_(path)
  , task_(task)
{
  const auto &local_list = model_->get_list();
  const Task *curr_t = &local_list[exe_path_[0]];
  auto size = exe_path_.size();
  for (u64 i{1}; i < size; ++i) {
    curr_t = &curr_t->child_tasks[exe_path_[i]];
  }
  undo_path_.push_back(curr_t->child_tasks.size());
}

void AddAction::execute()
{
  model_->add(task_, exe_path_);
}

void AddAction::undo()
{
  model_->remove(undo_path_);
}

StatusChangeAction::StatusChangeAction(Model &model, std::vector<u16> path,
                                       Status new_status)
  : Action(model, path)
  , new_status_(new_status)
{
  const auto &local_list = model_->get_list();
  const Task *curr_t = &local_list[exe_path_[0]];
  u64 size = exe_path_.size();
  for (u64 i{1}; i < size; ++i) {
    curr_t = &curr_t->child_tasks[exe_path_[i]];
  }
  old_status_ = curr_t->status;
}

void StatusChangeAction::execute()
{
  model_->change_task_status(exe_path_, new_status_);
}

void StatusChangeAction::undo()
{
  model_->change_task_status(exe_path_, old_status_);
}

PriorityChangeAction::PriorityChangeAction(Model &model, std::vector<u16> path,
                                           u16 new_priority)
  : Action(model, path)
  , new_priority_(new_priority)
{
  const auto &local_list = model_->get_list();
  const Task *curr_t = &local_list[exe_path_[0]];
  u64 size = exe_path_.size();
  for (u64 i{1}; i < size; ++i) {
    curr_t = &curr_t->child_tasks[exe_path_[i]];
  }
  old_priority_ = curr_t->priority;
}

void PriorityChangeAction::execute()
{
  model_->change_task_priority(exe_path_, new_priority_);
}

void PriorityChangeAction::undo()
{
  model_->change_task_priority(exe_path_, old_priority_);
}
}  // namespace Todo
