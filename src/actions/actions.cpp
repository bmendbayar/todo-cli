#include "actions/actions.h"

namespace todo {
Action::Action(Model &model, std::vector<u64> &&path)
  : model_(&model)
  , exe_path_(path)
{
}

Task *Action::find_task()
{
  std::vector<Task> local_list = model_->get_list();
  Task *curr_task = &local_list[exe_path_[0]];
  u64 size = exe_path_.size();
  for (u64 i{1}; i < size; ++i) {
    curr_task = &curr_task->child_tasks[exe_path_[i]];
  }
  return curr_task;
}

RemoveAction::RemoveAction(Model &model, std::vector<u64> &&path)
  : Action(model, std::move(path))
  , undo_path_(exe_path_)  // exe_path_ is initialized already
{
  undo_path_.pop_back();
  Task *curr_task = find_task();
  task_ = *curr_task;
}

void RemoveAction::execute()
{
  model_->remove(exe_path_);
}

void RemoveAction::undo()
{
  model_->add(task_, undo_path_);
}

AddAction::AddAction(Model &model, std::vector<u64> &&path, Task &&task)
  : Action(model, std::move(path))
  , undo_path_(exe_path_)
  , task_(task)
{
  Task *curr_task = find_task();
  undo_path_.push_back(curr_task->child_tasks.size());
}

void AddAction::execute()
{
  model_->add(task_, exe_path_);
}

void AddAction::undo()
{
  model_->remove(undo_path_);
}

StatusChangeAction::StatusChangeAction(Model &model, std::vector<u64> &&path, Status new_status)
  : Action(model, std::move(path))
  , new_status_(new_status)
{
  Task *curr_task = find_task();
  old_status_ = curr_task->status;
}

void StatusChangeAction::execute()
{
  model_->change_task_status(exe_path_, new_status_);
}

void StatusChangeAction::undo()
{
  model_->change_task_status(exe_path_, old_status_);
}

PriorityChangeAction::PriorityChangeAction(Model &model, std::vector<u64> &&path, u16 new_priority)
  : Action(model, std::move(path))
  , new_priority_(new_priority)
{
  Task *curr_task = find_task();
  old_priority_ = curr_task->priority;
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
