#define BOARD_SIZE 100

#include <stdbool.h>

void init_board();
void swap_board();
void reset_board();
bool get_square(int x, int y);
void write_square(int x, int y, bool state);
bool is_pos_valid(int x, int y);
void random_board();