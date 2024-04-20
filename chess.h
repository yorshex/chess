#ifndef _chess_h
#define _chess_h

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>

static const char *CHESS_INIT_FEN =
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

#define BOARD_ROWS 8
#define BOARD_COLS 8
#define BOARD_LEN (BOARD_ROWS*BOARD_COLS)

#define CA_WK (1<<0)
#define CA_WQ (1<<1)
#define CA_BK (1<<2)
#define CA_BQ (1<<3)

#define CHESS_AT(chess, x, y) ((chess)->board[(x)+(y)*BOARD_COLS])
#define CHESS_IS_POS_OUT(x, y) \
		((x) < 0 || (x) >= BOARD_COLS || (y) < 0 || (y) >= BOARD_COLS)
#define SGN(x) ((x > 0) - (0 > x))

typedef struct {
	char board[BOARD_LEN];
	int castle_avail;
	int passed_x, passed_y;
	int hmove_clock;
	int hmoves;
	int sel_x, sel_y;
} Chess;

// util.c
int imax(int a, int b);
int imin(int a, int b);
int iclamp(int n, int mn, int mx);
void print_chess(const Chess *chess);
int get_piece_index(char piece);

// logic.c
int is_figurine_char(int c);
bool is_there_obstacle(const Chess *chess, int x0, int y0, int n, int sx, int sy);
bool is_move_legal_unsafe(const Chess *chess, int x0, int y0, int x, int y);
bool is_move_legal(const Chess *chess, int x0, int y0, int x, int y);
bool perform_move(Chess* chess, int x0, int y0, int x, int y);

// input.c
void handle_mouse_input(Chess *chess, Rectangle board);

// fen_to_chess.c
int fen_to_chess(const char *fen, Chess *chess);

// draw.c
void draw_chessboard(const Chess *chess, Rectangle dest, Texture chessmen);

#endif // _chess_h
