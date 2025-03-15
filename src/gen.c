#include "board.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef TRACY_ENABLE
#include "TracyC.h"
#endif

int get_nb_neighbours(int square_x, int square_y){
#ifdef TRACY_ENABLE
    TracyCZoneS(ctx, 12, 1);
#endif

    int nb = 0;
    for (int x_offset = -1; x_offset <= 1; x_offset++){
        for (int y_offset = -1; y_offset <= 1; y_offset++){
                // exclude the same pos
                int x = square_x + x_offset;
                int y = square_y + y_offset;
                if (!(square_x == x && square_y == y)){
                    //printf("is_pos_valid(%d, %d) : %d\n", x, y, is_pos_valid(x, y));
                    if (is_pos_valid(x, y)){
                        nb += get_square(x, y);
                    }      
                }
        }
    }
    //printf("nb_neighbours : %d\n", nb);

#ifdef TRACY_ENABLE
    TracyCZoneEnd(ctx);
#endif

    return nb;
}

static void handle_square(int square_x, int square_y){
#ifdef TRACY_ENABLE
    TracyCZoneS(ctx, 11, 1);
#endif

    int nb_neighbours = get_nb_neighbours(square_x, square_y);
    if (get_square(square_x, square_y)){
        if (nb_neighbours != 2 && nb_neighbours != 3){
            write_square(square_x, square_y, false);
        }
    } else {
        if (nb_neighbours == 3){
            write_square(square_x, square_y, true);
        }
    }

#ifdef TRACY_ENABLE
    TracyCZoneEnd(ctx);
#endif
}

void do_generation(){
#ifdef TRACY_ENABLE
    TracyCZoneS(ctx, 10, 1);
#endif

    for (int x = 0; x < BOARD_SIZE; x++){
        for (int y = 0; y < BOARD_SIZE; y++){
            handle_square(x, y);
        }
    }
    swap_board();

#ifdef TRACY_ENABLE
    TracyCZoneEnd(ctx);
#endif
}