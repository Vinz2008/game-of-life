#include "load.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "raylib.h"
#include "gui.h"
#include "screen.h"
#include "board.h"
#include "error.h"

static bool load_menu = false;

static bool mouseOnText = false;

#define MAX_FILENAME_LENGTH 4096

static int filename_buf_length = 0; 
static char filename_buf[MAX_FILENAME_LENGTH];
static int save_button_state = 0; // 0 : NORMAL, 1 : MOUSE HOVER, 2 : PRESSED (TODO : use an enum ?)


bool is_in_load_menu(){
    return load_menu;
}

void set_load_menu(bool state){
    load_menu = state;
}

static const bool* load_board(const char* filename, bool zlib);

void handle_load_menu(){
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
            load_board(filename_buf, true);
            set_load_menu(false);
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

    DrawText("LOAD", start_x, 140, 40, ORANGE);

    // TODO : add blinker in text box (https://github.com/raysan5/raylib/blob/master/examples/text/text_input_box.c)
    DrawRectangleRec(text_box, LIGHTGRAY);
    DrawText(filename_buf, (int)text_box.x + 5, (int)text_box.y + 8, 40, MAROON);


    DrawRectangleRec(save_button_box, BLACK);
    DrawText("Load", save_button_box.x + save_button_box.width/4, save_button_box.y+10, 30, WHITE);
}

static const bool* load_board(const char* filename, bool zlib){
    FILE* f = fopen(filename, "r");
    if (!f){
        ERROR("opening file failed\n");
    }

    size_t current_board_size = BOARD_SIZE * BOARD_SIZE;
    //fread(&current_board_size, sizeof(size_t), 1, f);


    bool* buf = malloc(current_board_size * sizeof(bool));
    if (!buf){
        ERROR("error in memory allocation\n");
    }

    if (zlib){
        fseek(f, 0, SEEK_END);
        size_t file_size = ftell(f);
        rewind(f);
        char* uncompressed_buf = malloc(file_size);
        if (!uncompressed_buf){
            ERROR("error in memory allocation\n");
        }
        fread(uncompressed_buf, sizeof(char), file_size, f);
        size_t destLen;
        int res = uncompress((Bytef*)buf, &destLen, (Bytef*)uncompressed_buf, file_size * sizeof(char));
        if (res != Z_OK){
            ERROR("error in zlib uncompress : %d\n", res);
        }
        free(uncompressed_buf);
    } else {
        fread(buf, sizeof(bool), current_board_size, f);
    }

    fclose(f);

    return buf;
}