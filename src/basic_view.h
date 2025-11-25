#pragma once

#include "view.h"

namespace Todo
{
class BasicView : public View
{
public:
  BasicView() = default;

  virtual MenuOptions get_menu_opt() override;

  virtual std::string get_task_desc(const std::string &msg) override;

  virtual size_t get_index(const std::string &msg) override;

  virtual void display_list(const std::vector<Todo::Task> &todo_list,
                            size_t level = 0) override;

  virtual void display_msg(const std::string &msg) override;
};
}  // namespace Todo
