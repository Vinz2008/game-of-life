#include "board.h"
#include <string.h>
#include <time.h>
#include "raylib.h"

static bool board[BOARD_SIZE * BOARD_SIZE];
static bool board2[BOARD_SIZE * BOARD_SIZE];

static bool* current_board;
static bool* next_board;

void reset_board(){
    memset(current_board, 0, BOARD_SIZE*BOARD_SIZE);
    memset(next_board, 0, BOARD_SIZE*BOARD_SIZE);
}

static int get_board_pos(int x, int y){
    return y * BOARD_SIZE + x;
}

bool get_square(int x, int y){
    return current_board[get_board_pos(x, y)];
} 

void write_square(int x, int y, bool state){
    next_board[get_board_pos(x, y)] = state;
}


void init_board(){
    current_board = board;
    next_board = board2;
}

void swap_board(){
    bool* temp = current_board;
    current_board = next_board;
    next_board = temp;
    memcpy(next_board, current_board, BOARD_SIZE*BOARD_SIZE);
}

bool is_pos_valid(int x, int y){
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

void random_board(){
    SetRandomSeed(time(NULL));
    for (int x = 0; x < BOARD_SIZE; x++){
        for (int y = 0; y < BOARD_SIZE; y++){
            if (GetRandomValue(0, 10) > 5){
                write_square(x, y, true);
            }
        }
    }
}

void blinker(int x, int y){
    for (int x_offset = -1; x_offset <= 1; x_offset++){
        write_square(x + x_offset, y, true);
    }
}