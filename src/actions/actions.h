#pragma once

#include <vector>

#include "model.h"
#include "types.h"

namespace Todo {
class Action {
protected:
  Model *model_{nullptr};
  std::vector<u16> exe_path_;

public:
  Action(Model &model, std::vector<u16> path);
  virtual ~Action() = default;
  virtual void execute() = 0;
  virtual void undo() = 0;
};

class RemoveAction : public Action {
private:
  std::vector<u16> undo_path_;
  Task task_;

public:
  RemoveAction(Model &model, std::vector<u16> path);
  virtual void execute() override;
  virtual void undo() override;
};

class AddAction : public Action {
private:
  std::vector<u16> undo_path_;
  Task task_;

public:
  AddAction(Model &model, std::vector<u16> path, Task task);
  virtual void execute() override;
  virtual void undo() override;
};

class StatusChangeAction : public Action {
private:
  Status new_status_;
  Status old_status_;

public:
  StatusChangeAction(Model &model, std::vector<u16> path, Status new_status);
  virtual void execute() override;
  virtual void undo() override;
};

class PriorityChangeAction : public Action {
private:
  u16 new_priority_;
  u16 old_priority_;

public:
  PriorityChangeAction(Model &model, std::vector<u16> path, u16 new_priority);
  virtual void execute() override;
  virtual void undo() override;
};
}  // namespace Todo
