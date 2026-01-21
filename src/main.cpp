#include <iostream>

#include "controller.h"

int main(int argc, char **argv)
{
    try {
        todo::Controller app(argc, argv);
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what();
    }
    return 0;
}
