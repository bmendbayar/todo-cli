#include "actions/actions.h"

namespace todo {
Action::Action(Model &model, std::vector<u64> &&path)
    : model_(&model)
    , exe_path_(std::move(path))
{
}

RemoveAction::RemoveAction(Model &model, std::vector<u64> &&path)
    : Action(model, std::move(path))
{
    Task *curr_task = model_->get_task(exe_path_);
    if (curr_task != nullptr) {
        task_ = *curr_task;
    }
}

void RemoveAction::execute()
{
    model_->remove(exe_path_);
}

void RemoveAction::undo()
{
    model_->add(task_, exe_path_);
}

AddAction::AddAction(Model &model, std::vector<u64> &&path, Task &&task)
    : Action(model, std::move(path))
    , task_(std::move(task))
{
    Task *curr_task = model_->get_task(exe_path_);
    if (curr_task != nullptr) {
        exe_path_.push_back(curr_task->child_tasks.size());
    }
}

void AddAction::execute()
{
    model_->add(task_, exe_path_);
}

void AddAction::undo()
{
    model_->remove(exe_path_);
}

StatusChangeAction::StatusChangeAction(
    Model &model, std::vector<u64> &&path, Status new_status
)
    : Action(model, std::move(path))
    , new_status_(new_status)
{
    Task *curr_task = model_->get_task(exe_path_);
    if (curr_task != nullptr) {
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

PriorityChangeAction::PriorityChangeAction(
    Model &model, std::vector<u64> &&path, u16 new_priority
)
    : Action(model, std::move(path))
    , new_priority_(new_priority)
{
    Task *curr_task = model_->get_task(exe_path_);
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
