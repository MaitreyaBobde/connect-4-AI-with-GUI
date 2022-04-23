# connect-4-AI-with-GUI

This programs makes use of the Alpha-Beta variant of the MiniMax algorithm to find the best possible move for the computer to make for given state of the board. Details of the Algorithm can be found on the [wikipedia page](https://en.wikipedia.org/wiki/Alpha–beta_pruning). you can find many similarities between the pseudo code and [my implementation](src/include/connect4.h) of the alogrithm.

It also uses SDL2 library for the Graphical interpretation of connect 4 board which the user can interact with to play the next move with proper animations and Win, Draw and Lose states.

complete program is written in c++ and can be compiled with the makefile provided and compiled application can be found and run in the build folder (it requires the dependency files for the sdl libraries to run, so is recommended to execute inside the build folder).

<img src="https://user-images.githubusercontent.com/78481858/164889607-3e1fc3cf-cedb-417b-a028-99017567bbe1.gif" alt="gif" width="200" align="center" />
