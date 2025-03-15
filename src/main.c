#include "raylib.h"
//#include "raymath.h"
//#include "rlgl.h"
#include <stdio.h>
#include "board.h"
#include "gen.h"
#include "mouse.h"

// TODO : use haslife for better performance


int screen_height = 1000;
int screen_width = 1000;


#define BG_BLACK (Color){ 10, 10, 10, 255 }

static void printBoard(Camera2D camera){

    BeginMode2D(camera);

        for (int x = 0; x < BOARD_SIZE; x++){
            for (int y = 0; y < BOARD_SIZE; y++){
                //Color color = BG_BLACK;
                if (get_square(x, y)){
                    //color = WHITE;
                    Color color = WHITE;
                    DrawRectangle(x*10, y*10, 10, 10, color);
                }
                //DrawRectangle(x*10, y*10, 10, 10, color);
            }
        }
    

        for (int x = 0; x < BOARD_SIZE; x++){
            DrawLine(x * 10, 0, x * 10, BOARD_SIZE * 10, GRAY);
        }

        for (int y = 0; y < BOARD_SIZE; y++){
            DrawLine(0, y * 10, BOARD_SIZE * 10, y * 10, GRAY);
        }

    EndMode2D();

    //DrawLine()
}

enum MouseMode {
    MOUSE_ZOOM,
    MOUSE_MOVE,
};


#define GEN_TIMEOUT 1000

int main(void)
{
    init_board();

    reset_board();

    //random_board();
    
    blinker(BOARD_SIZE/2, BOARD_SIZE/2);


    InitWindow(screen_height, screen_width, "raylib [core] example - basic window");

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

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


        handle_mouse_move(&camera);

        handle_mouse_zoom(&camera);


        BeginDrawing();
            ClearBackground(BG_BLACK);
            printBoard(camera);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}