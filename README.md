# Battleship
A terminal-based implementation of the battleship game written in C, which features:
- Random generation of the ships on the board.
- Playing against computer.
- Saving games.
- Displaying saves and loading a game to continue playing.

![Screenshot of main menu](/extra/main_menu.png)
![Screenshot of load menu](/extra/load_menu.png)
![Screenshot of game](/extra/gameplay.png)

## Gameplay
- Navigate the main and/or load menus by entering the corresponding number.
- Once a new game is started, enter your name and start making moves:
  - _save_ to save the game;
  - _exit_ to exit to the main menu;
  - coordinates like _A2_ or _g5_ to shoot at the enemy board.
- The result of the turn and instructions what to do will be displayed at the bottom of the screen.
- Your goal is to destroy all enemy ships before they do the same to yours.
- When in the load menu, you can:
  - list all found saved games;
  - enter a user name and list all games of that player;
  - display the state of a saved game with given ID;
  - continue playing a saved game with given ID.
- Saves persist between sessions via a savefile; you should not modify it manually.

## Building
No external dependecies other than the C standard library. Only a C compiler is required (C99 is enough).
A Makefile is provided for systems that support the make build system, otherwise compile all .c files in src/ directory into an executable.

## Notes
- Make sure your terminal window is large enough so that the lines do not wrap.
- The game uses ANSI escape codes, so your terminal must support it. (most do)
  - If not, try to install a different one.
  - Or on Windows 10+, you can enable support (may be off by default) by adding a DWORD registry key called _VirtualTerminalLevel_ with value 1 to *HKEY_CURRENT_USER\Console\\* (if it does not yet exist).