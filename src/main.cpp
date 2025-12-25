#include <ncurses.h>

#include "controller.h"

inline bool pre_order_trav(const std::vector<Todo::Task> &list, int &curr,
                           const int target, std::vector<Todo::U16> &path)
{
  for (Todo::U16 i{}; i < list.size(); i++)
  {
    path.push_back(i);
    curr++;

    if (curr == target)
      return true;

    if (list[i].child_tasks.empty() == false)
      if (pre_order_trav(list[i].child_tasks, curr, target, path))
        return true;

    path.pop_back();
  }

  return false;
}

int main(int argc, char **argv)
{
  Todo::Controller app(argc, argv);
  app.run();

  // Todo::Model model;
  // int x{};
  // std::vector<Todo::U16> path;
  // pre_order_trav(model.get_list(), x, 5, path);
  // model.remove(path);

  return 0;
}
