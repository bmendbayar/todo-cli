#include <ncurses.h>

#include "controller.h"

int main(int argc, char **argv)
{
    Todo::Controller app(argc, argv);
    app.run();
    return 0;
}
