#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "menu.h"
#include "utility.h"

int main(void) {
    int is_playing = 0, whose_turn = 1;
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
            whose_turn = 1;
            // the main gameplay loop
            // all conitnue`s inside refer to this while
            while (is_playing) {
                clear_screen();
                print_2_boards(&player_board, &computer_board, "Your side",
                               "Opponent's side");
                printf("\n");
                printf("%s", extra_message);
                if (whose_turn == 2) {
                    printf("Press enter to continue\n");
                    flush_stdin();
                    whose_turn = 0;
                } else if (whose_turn == 1) {
                    printf("Your turn (coords, \"exit\"): ");

                    switch (get_turn_input(&y, &x)) {
                    case TURN_INVALID:
                        extra_message = "Not a valid turn, try again.\n";
                        continue;
                    case TURN_EXIT:
                        is_playing = 0;
                        continue;
                    case TURN_COORDS:
                        switch (take_shot(&player_board, y, x)) {
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
                            whose_turn = 2;
                            continue;
                        case SHOT_WIN:
                            clear_screen();
                            print_2_boards(&player_board, &computer_board,
                                           "Your side (Winner)",
                                           "Opponent's side");
                            printf("\nCongratulations! You win!\n");
                            printf("Press enter to go back to main menu\n");
                            flush_stdin();
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
                } else {
                    // computer's turn
                    // only reachable if player shot validly but hit water
                    switch (computer_take_shot(&computer_board)) {
                    case SHOT_HIT:
                        extra_message =
                            "Opponent hit the ship and takes an extra turn.\n";
                        whose_turn = 2;
                        continue;
                    case SHOT_SHIP_DOWN:
                        extra_message =
                            "Opponent sunk the ship and takes an extra turn.\n";
                        whose_turn = 2;
                        continue;
                    case SHOT_MISSED:
                        extra_message = "Opponent hit the water (a miss). \n";
                        whose_turn = 1;
                        continue;
                    case SHOT_WIN:
                        clear_screen();
                        print_2_boards(&player_board, &computer_board,
                                       "Your side", "Opponent's side (Winner)");
                        printf("\nOppenent wins! Better luck next time.\n");
                        printf("Press enter to go back to main menu\n");
                        flush_stdin();
                        is_playing = 0;
                        continue;
                    // unexpected, computer_take_shot() supposed to allways
                    // return one of above
                    default:
                        exit_game(1);
                        break;
                    }
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