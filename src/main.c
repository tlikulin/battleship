#include <stdio.h>
#include <unistd.h>

#include "board.h"
#include "menu.h"
#include "utility.h"

int main(void) {
    int choice, is_playing = 0;
    int y = -1, x = -1;
    const char* extra_message = "";
    board_t player_board, computer_board;

    if (!isatty(0)) {
        printf("This app is for interactive usage only (no pipe)\n");
        exit_game(1);
    }

    // main app loop (infinite)
    while (1) { 
        choice = run_main_menu();
        
        switch (choice) {
        case CHOICE_MM_EXIT:
            exit_game(0);
            break;
        case CHOICE_MM_NEW_GAME:
            init_board(&player_board);
            init_board(&computer_board);
            extra_message = "\n";
            is_playing = 1;
            // the main gameplay loop
            while (is_playing) {
                clear_screen();
                print_2_boards(&player_board, &computer_board, "Your side", "Opponent's side");
                printf("\n");

                printf("%s", extra_message);
                printf("Your turn (coords, \"exit\") %d: ", player_board.targets_left);

                choice = get_turn_input(&y, &x);
                switch (choice) {
                case TURN_INVALID:
                    extra_message = "Not a valid turn, try again.\n";
                    continue;
                case TURN_EXIT:
                    is_playing = 0;
                    break;
                case TURN_COORDS:
                    switch (take_shot(&player_board, y, x)) {
                    case SHOT_HIT:
                        extra_message = "You hit the ship.\n";
                        break;
                    case SHOT_ALREADY:
                        extra_message = "You already fired at this tile before. Try again.\n";
                        break;
                    case SHOT_INVALID:
                        extra_message = "Invalid coordinates to fire at.\n";
                        break;
                    case SHOT_SHIP_DOWN:
                        extra_message = "You sunk the ship!\n";
                        break;
                    case SHOT_MISSED:
                        extra_message = "You hit the water (a miss). \n";
                        break;
                    case SHOT_WIN:
                        clear_screen();
                        print_2_boards(&player_board, &computer_board, "Your side (Winner)", "Opponent's side");
                        printf("\nCongratulations! You win!\nPress enter to go back to main menu\n");
                        getchar();
                        flush_stdin();
                        is_playing = 0;
                        break;
                    default:
                        exit_game(1);
                        break;
                    }
                    break;
                default:
                    exit_game(1);
                    break;
                }
            }
            break;
        // unexpected, run_main_menu() supposed to allways return one of above
        default:
            exit_game(1);
            break;
        }
    }
}