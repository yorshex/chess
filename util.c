#include "chess.h"

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

int imax(int a, int b) { return a > b ? a : b; }
int imin(int a, int b) { return a < b ? a : b; }
int iclamp(int n, int mn, int mx) { return imax(imin(n, mx), mn); }

void print_chess(const Chess *chess)
{
	eprintf(" move %i %c\n",
	       chess->hmoves / 2,
	       chess->hmoves % 2 ? 'b' : 'w');
	for (int y = 0; y < BOARD_ROWS; y++) {
		eprintf("%i|", 8 - y);
		for (int x = 0; x < BOARD_COLS; x++) {
			eprintf(" %c", CHESS_AT(chess, x, y));
		}
		eprintf("\n");
	}
	eprintf(" +----------------\n");
	eprintf("   a b c d e f g h\n");
}

int get_piece_index(char piece)
{
	piece = tolower(piece);

	switch(piece) {
		default : return -1;
		case 'p': return 0;
		case 'r': return 1;
		case 'n': return 2;
		case 'b': return 3;
		case 'q': return 4;
		case 'k': return 5;
	};
}
