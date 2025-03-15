#include "save.h"
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <stdlib.h>
#include "raylib.h"
#include "screen.h"
#include "board.h"

#define BG_GRAY (Color){ 220, 220, 220, 255 }

static bool save_menu = false;

static bool mouseOnText = false;

#define MAX_FILENAME_LENGTH 4096

static int filename_buf_length = 0; 
static char filename_buf[MAX_FILENAME_LENGTH];
static int save_button_state = 0; // 0 : NORMAL, 1 : MOUSE HOVER, 2 : PRESSED (TODO : use an enum ?)
//static bool is_save_button_pressed = false;

static void save_board(const char* filename, bool zlib);

void handle_save_menu(){
    const int screen_width = get_screen_width();
    const int screen_height = get_screen_height();
    int start_x = screen_width/2;
    int start_y = screen_height/2;

    const int text_box_width = 225;
    
    Rectangle save_button_box = { start_x, start_y + 60, text_box_width, 50 };

    if (CheckCollisionPointRec(GetMousePosition(), save_button_box))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            save_button_state = 2; 
        } else { 
            save_button_state = 1; 
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            //is_save_button_pressed = true;
            save_board(filename_buf, true);
            set_save_menu(false);
            printf("save button pressed\n");
        }
    } else { 
        save_button_state = 0; 
    }


    Rectangle text_box = { start_x, start_y, text_box_width, 50 };
    if (CheckCollisionPointRec(GetMousePosition(), text_box)){
         mouseOnText = true;
    } else { 
        mouseOnText = false;
    }

    if (mouseOnText){
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        int key = GetCharPressed();
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (filename_buf_length < MAX_FILENAME_LENGTH))
            {
                filename_buf[filename_buf_length] = (char)key;
                filename_buf[filename_buf_length+1] = '\0';
                filename_buf_length++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            filename_buf_length--;
            if (filename_buf_length < 0){
                 filename_buf_length = 0;
            }
            filename_buf[filename_buf_length] = '\0';
        }

    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    // TODO : add check box for zlib


    Rectangle background_box = { 10, 10, screen_width-20, screen_height - 20 };
    DrawRectangleRec(background_box, BG_GRAY);

    DrawText("SAVE", start_x, 140, 40, ORANGE);

    // TODO : add blinker in text box (https://github.com/raysan5/raylib/blob/master/examples/text/text_input_box.c)
    DrawRectangleRec(text_box, LIGHTGRAY);
    DrawText(filename_buf, (int)text_box.x + 5, (int)text_box.y + 8, 40, MAROON);


    DrawRectangleRec(save_button_box, BLACK);
    DrawText("Save", save_button_box.x + save_button_box.width/4, save_button_box.y+10, 30, WHITE);
}

void set_save_menu(bool state){
    save_menu = state;
    if (state){
        filename_buf_length = 0;
        memset(filename_buf, 0, MAX_FILENAME_LENGTH);
    }
}

bool is_in_save_menu(){
    return save_menu;
}

static void save_board(const char* filename, bool zlib){
    FILE* f = fopen(filename, "w");
    const bool* current_board = get_current_board();
    const size_t current_board_size = BOARD_SIZE * BOARD_SIZE;
    const bool* buf = current_board;
    size_t buf_size = current_board_size;
    unsigned char* board_compressed = NULL;
    if (zlib){
        size_t zlib_upper_bound = compressBound(sizeof(bool) * current_board_size);
        board_compressed = malloc(zlib_upper_bound);
        compress(board_compressed, &buf_size, (unsigned char*)current_board, current_board_size);
    }

    fwrite(buf, sizeof(bool), buf_size, f);

    if (board_compressed){
        free(board_compressed);
    }
    fclose(f);
}