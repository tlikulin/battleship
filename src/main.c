#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "menu.h"
#include "utility.h"

enum turn {
    TURN_NONE = 0,
    TURN_PLAYER,
    TURN_COMPUTER,
    // required before a computer turn to ask user for enter
    // and not immediately make computer turn(s)
    TURN_PASSING,
};

int main(void) {
    int is_playing = 0;
    enum turn whose_turn = TURN_NONE;
    int y = -1, x = -1;
    const char* extra_message = "";
    board_t player_board, computer_board;

    if (!isatty(0)) {
        printf("This app is for interactive usage only (no pipe)\n");
        exit_game(1);
    }

    srand(time(NULL));

    // main app loop (infinite)
    while (1) {
        switch (run_main_menu()) {
        case CHOICE_MM_EXIT:
            exit_game(0);
            break;
        case CHOICE_MM_NEW_GAME:
            init_board(&player_board);
            init_board(&computer_board);
            extra_message = "\n";
            is_playing = 1;
            whose_turn = TURN_PLAYER;
            // the main gameplay loop
            // all conitnue`s inside refer to this while
            while (is_playing) {
                clear_screen();
                print_2_boards(&player_board, &computer_board, "Your side",
                               "Opponent's side", 1);
                printf("\n");
                printf("%s", extra_message);
                switch (whose_turn) {
                case TURN_PASSING:
                    printf("Press enter to continue");
                    flush_stdin(); // wait for enter
                    whose_turn = TURN_COMPUTER;
                    break;
                case TURN_PLAYER:
                    printf("Your turn (coords, \"exit\"): ");

                    switch (get_turn_input(&y, &x)) {
                    case INPUT_INVALID:
                        extra_message = "Not a valid turn, try again.\n";
                        continue;
                    case INPUT_EXIT:
                        is_playing = 0;
                        continue;
                    case INPUT_COORDS:
                        switch (take_shot(&computer_board, y, x)) {
                        case SHOT_HIT:
                            extra_message =
                                "You hit the ship. Take an extra turn.\n";
                            continue;
                        case SHOT_ALREADY:
                            extra_message = "You already fired at this tile "
                                            "before. Try again.\n";
                            continue;
                        case SHOT_INVALID:
                            extra_message =
                                "Invalid coordinates to fire at. Try again\n";
                            continue;
                        case SHOT_SHIP_DOWN:
                            extra_message =
                                "You sunk the ship! Take an extra turn.\n";
                            continue;
                        case SHOT_MISSED:
                            extra_message = "You hit the water (a miss).\n";
                            whose_turn = TURN_PASSING;
                            continue;
                        case SHOT_WIN:
                            clear_screen();
                            print_2_boards(&player_board, &computer_board,
                                           "Your side (Winner)",
                                           "Opponent's side", 1);
                            printf("\nCongratulations! You win!\n");
                            printf("Press enter to go back to main menu");
                            flush_stdin(); // wait for enter
                            is_playing = 0;
                            continue;
                        default:
                            exit_game(1);
                            break;
                        }
                        break;
                    default:
                        exit_game(1);
                        break;
                    }
                    break;
                    // computer's turn
                    // only reachable if player shot validly but hit water
                case TURN_COMPUTER:
                    switch (computer_take_shot(&player_board)) {
                    case SHOT_HIT:
                        extra_message =
                            "Opponent hit the ship and takes an extra turn.\n";
                        whose_turn = TURN_PASSING;
                        continue;
                    case SHOT_SHIP_DOWN:
                        extra_message =
                            "Opponent sunk the ship and takes an extra turn.\n";
                        whose_turn = TURN_PASSING;
                        continue;
                    case SHOT_MISSED:
                        extra_message = "Opponent hit the water (a miss). \n";
                        whose_turn = TURN_PLAYER;
                        continue;
                    case SHOT_WIN:
                        clear_screen();
                        print_2_boards(&player_board, &computer_board,
                                       "Your side", "Opponent's side (Winner)",
                                       1);
                        printf("\nOppenent wins! Better luck next time.\n");
                        printf("Press enter to go back to main menu");
                        flush_stdin(); // wait for enter
                        is_playing = 0;
                        continue;
                    // unexpected, computer_take_shot() is supposed to allways
                    // return one of above
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