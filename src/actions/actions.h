#pragma once

#include <vector>

#include "model.h"
#include "types.h"

namespace todo {
class Action {
protected:
  Model *model_{nullptr};      ///< Model to perform actions on.
  std::vector<u64> exe_path_;  ///< Path of execution.

  /// \brief Finds specified task.
  Task *find_task();

public:
  /// \brief Parameterized constructor.
  /// \param model Reference to the model to work with.
  /// \param path R-value reference to the path (execution) of action.
  Action(Model &model, std::vector<u64> &&path);

  /// \brief Default destructor.
  virtual ~Action() = default;

  /// \brief Execute action.
  virtual void execute() = 0;

  /// \brief Undo action.
  virtual void undo() = 0;
};

class RemoveAction : public Action {
private:
  std::vector<u64> undo_path_{};  ///< Path of undo.
  Task task_{};                   ///< Removed task.

public:
  /// \brief Parameterized constructor.
  /// \param model Reference to the model to work with.
  /// \param path R-value reference to the path (execution) of action.
  RemoveAction(Model &model, std::vector<u64> &&path);

  /// \brief execute action.
  virtual void execute() override;

  /// \brief undo action.
  virtual void undo() override;
};

class AddAction : public Action {
private:
  std::vector<u64> undo_path_{};  ///< Path of undo.
  Task task_{};                   ///< Added task.

public:
  /// \brief Parameterized constructor.
  /// \param model Reference to the model to work with.
  /// \param path R-value reference to the path (execution) of action.
  /// \param task R-value reference to the task to add.
  AddAction(Model &model, std::vector<u64> &&path, Task &&task);

  /// \brief execute action.
  virtual void execute() override;

  /// \brief undo action.
  virtual void undo() override;
};

class StatusChangeAction : public Action {
private:
  Status new_status_{};  ///< New status of task.
  Status old_status_{};  ///< Old status of task.

public:
  /// \brief Parameterized constructor.
  /// \param model Reference to the model to work with.
  /// \param path R-value reference to the path (execution) of action.
  /// \param new_status The new status to be assigned to task.
  StatusChangeAction(Model &model, std::vector<u64> &&path, Status new_status);

  /// \brief execute action.
  virtual void execute() override;

  /// \brief undo action.
  virtual void undo() override;
};

class PriorityChangeAction : public Action {
private:
  u16 new_priority_{};  ///< New priority of task.
  u16 old_priority_{};  ///< Old priority of task.

public:
  /// \brief Parameterized constructor.
  /// \param model Reference to the model to work with.
  /// \param path R-value reference to the path (execution) of action.
  /// \param new_priority The new priority to be assigned to task.
  PriorityChangeAction(Model &model, std::vector<u64> &&path, u16 new_priority);

  /// \brief execute action.
  virtual void execute() override;

  /// \brief undo action.
  virtual void undo() override;
};
}  // namespace todo
