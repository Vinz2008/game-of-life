#include "raylib.h"
#include <stdio.h>
#include "board.h"
#include "gen.h"

// TODO : use haslife for better performance


int screen_height = 1000;
int screen_width = 1000;


#define BG_BLACK (Color){ 10, 10, 10, 255 }

static void printBoard(){
    for (int x = 0; x < BOARD_SIZE; x++){
        for (int y = 0; y < BOARD_SIZE; y++){
            Color color = BG_BLACK;
            if (get_square(x, y)){
                color = WHITE;
            }
            DrawRectangle(x*10, y*10, 10, 10, color);
        }
    }

    for (int x = 0; x < BOARD_SIZE; x++){
        DrawLine(x * 10, 0, x * 10, screen_height, GRAY);
    }

    for (int y = 0; y < BOARD_SIZE; y++){
        DrawLine(0, y * 10, screen_width, y * 10, GRAY);
    }

    //DrawLine()
}


#define GEN_TIMEOUT 500

int main(void)
{
    init_board();

    reset_board();

    //random_board();
    blinker(screen_width/20, screen_height/20);


    InitWindow(screen_height, screen_width, "raylib [core] example - basic window");

    float gen_tick = 0;

    int gen_nb = 1;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime() * GEN_TIMEOUT;
        gen_tick += delta;
        if (gen_tick > GEN_TIMEOUT){
            gen_tick -= GEN_TIMEOUT;
            printf("gen %d\n", gen_nb);
            do_generation();
            gen_nb++;
        }
        BeginDrawing();
            ClearBackground(BLACK);
            printBoard();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}