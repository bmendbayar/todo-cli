# CLI Todo

A persistent, hierarchical command-line To-Do application allowing infinite subtasks, built with a flexible architecture to support both rich (Ncurses) and standard text interfaces. See documentation [here](https://yuchyahav.github.io/cli-todo/).

## Demo

![Project Demo](assets/demo.gif)

## Architectural Design

MVC Pattern I chose Model-View-Controller (MVC) to decouple data management from the user interface, allows for swapping between a basic text UI and a rich terminal UI without changing core logic.

* Model: Manages the recursive Task data structure and handles JSON file I/O using Boost.JSON.
* View: An abstract base class defining the interface.
  * BasicView: Standard text-based I/O.
  * IView: Ncurses-based TUI for an interactive experience.
* Controller: Connects the two, parsing user input strings (like paths "1 2") into model commands.

![MVC Diagram](assets/mvc_diagram.png)


## Key Features

* Hierarchical Tasks: Task structs contain a vector of children, enabling infinite nesting.
* Persistence: Auto-saves/loads to .todo/todo_list.json to prevent data loss.
* Dual Interface: Run with -b for basic mode or default to the Ncurses windowed interface.

## Roadmap

- [ ] Task priority
- [ ] Undo/redo
- [ ] Copy/pase
- [ ] Due dates
- [ ] Task inheritance
- [ ] Vi view

## Installation

Clone the repository and install dependencies:
### Dependencies
* ncurses
* boost
* cmake
* C compiler

```bash
git clone https://github.com/yuchyahav/cli-todo.git
cd cli-todo
mkdir build
cd build
cmake ..
make
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
