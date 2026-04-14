#ifndef BUBBLEBLAST_H   // Se non già fatto, definisci questi header fino a endif
#define BUBBLEBLAST_H

#define ROWS 5
#define COLS 6
#define MAX_DEPTH 8

// Posizione del cursore

struct Move {
    int row;
    int col;
};


enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum CellState {
    EMPTY,
    BUBBLE_SMALL,
    BUBBLE_LARGE,
    NO_BUBBLE
};

void explode(int i, int j, enum CellState (*board)[COLS]);
void change_state(int i, int j, enum CellState (*board)[COLS]);
void initializeBoard(enum CellState (*board)[COLS]);
void displayBoard(int cursorRow, int cursorCol, enum CellState (*board)[COLS]);
int is_game_over(enum CellState board[ROWS][COLS]);
void moveCursor(int *cursorRow, int *cursorCol, enum Direction direction);
// Sostituisci la riga 88 nel .c e la riga 36 nel .h con questa:
int find_winning_move_at_depth(int depth, enum CellState board[ROWS][COLS], struct Move *moves, int *num_moves);
#endif