#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdio.h>
#include "board.h"
#include "gen.h"

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
            DrawLine(x * 10, 0, x * 10, screen_height, GRAY);
        }

        for (int y = 0; y < BOARD_SIZE; y++){
            DrawLine(0, y * 10, screen_width, y * 10, GRAY);
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
    blinker(screen_width/20, screen_height/20);


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


        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            printf("mouse left\n");
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        float wheel = GetMouseWheelMove();
        if (wheel != 0){
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set the offset to where the mouse is
            camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point 
            // under the cursor to the screen space point under the cursor at any zoom
            camera.target = mouseWorldPos;

            // Zoom increment
            float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
            if (wheel < 0){ 
                scaleFactor = 1.0f/scaleFactor; 
            }
            camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
        }


        BeginDrawing();
            ClearBackground(BG_BLACK);
            printBoard(camera);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}