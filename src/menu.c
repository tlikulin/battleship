#include <stdio.h>

#include "menu.h"
#include "utility.h"

// LOCAL:

// the logo displayed in the main menu
// Original is in extra/battleship_figlet.txt
const char* BATTLESHIP_LOGO =
    " ____        _   _   _          _     _            \n"
    "| __ )  __ _| |_| |_| | ___ ___| |__ (_)_ __       \n"
    "|  _ \\ / _` | __| __| |/ _ / __| '_ \\| | '_ \\   \n"
    "| |_) | (_| | |_| |_| |  __\\__ | | | | | |_) |    \n"
    "|____/ \\__,_|\\__|\\__|_|\\___|___|_| |_|_| .__/  \n"
    "                                       |_|         \n";

const char* MAIN_MENU_CHOICES = "1.) Play a new game \n"
                                "2.) Load already saved game \n"
                                "3.) Exit the game \n";

// EXPOSED in menu.c:

int run_main_menu(void) {
    int choice = CHOICE_MM_NONE;

    clear_screen();
    printf("%s%s", BATTLESHIP_LOGO, MAIN_MENU_CHOICES);
    choice = get_int();

    while (choice != CHOICE_MM_NEW_GAME && choice != CHOICE_MM_EXIT) {
        clear_screen();
        printf("%s%s", BATTLESHIP_LOGO, MAIN_MENU_CHOICES);

        if (choice == CHOICE_MM_LOAD) {
            printf("Not yet implemented!\n");
        }

        printf("You made an invalid choice, try again.\n");

        choice = get_int();
    }

    return choice;
}
