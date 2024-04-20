#include "chess.h"

#define ADD_I \
	do { i++; if (!fen[i]) return 0; } while (0)
#define ADD_I_SPACE \
	do { i++; if (!fen[i] || fen[i] != ' ') return 0; } while (0)

int fen_to_chess(const char *fen, Chess *chess)
{
	int row = 0;
	int col = 0;
	int i = 0;
	int wasdigit = 0;
	int fmvn = 0;
	int hmvc = 0;
	int py = 0;
	int px = 0;

	for (; fen[i] != ' ' && fen[i]; i++) {
		if (fen[i] == '/') {
			if (col != BOARD_COLS) return 0;

			row++;
			if (row >= BOARD_ROWS) return 0;

			col = 0;

			wasdigit = 0;
			continue;
		}

		if (col >= BOARD_COLS) return 0;

		if (is_figurine_char(fen[i])) {
			CHESS_AT(chess, col++, row) = fen[i];
			wasdigit = 0;
		}
		else if (fen[i] != '0' && isdigit(fen[i])) {
			if (wasdigit) return 0;

			int num = (unsigned char)(fen[i] - '0');

			col += num;
			if (col > BOARD_COLS) return 0;

			for (int i = 0; i < num; i++)
				CHESS_AT(chess, col-1-i, row) = '.';

			wasdigit = 1;
		}
		else {
			return 0;
		}
	}

	if (!fen[i]) return 1; // fen may not have the other part

	else if (fen[i] != ' ') return 0;
	ADD_I;

	// moving side
	if (fen[i] == 'b') chess->hmoves = 1;
	else if (fen[i] == 'w') chess->hmoves = 0;
	else return 0;
	ADD_I_SPACE;
	ADD_I;

	// castling availability
	if (fen[i] != '-') {
		for (; fen[i] != ' ' && fen[i]; i++) {
			if      (fen[i] == 'K') chess->castle_avail |= CA_WK;
			else if (fen[i] == 'Q') chess->castle_avail |= CA_WQ;
			else if (fen[i] == 'k') chess->castle_avail |= CA_BK;
			else if (fen[i] == 'q') chess->castle_avail |= CA_BQ;
			else return 0;
		}
	}
	else {
		chess->castle_avail = 0;
		ADD_I_SPACE;
	}
	ADD_I;

	// en passen square
	if (fen[i] != '-') {
		if (!isalpha(fen[i]) || !islower(fen[i])) return 0;
		py = (unsigned char)(fen[i] - 'a');
		if (py < 0 || py >= BOARD_ROWS) return 0;
		ADD_I;

		if (!isdigit(fen[i])) return 0;
		px = (unsigned char)(fen[i] - '0');
		if (px < 0 || px >= BOARD_ROWS) return 0;

		chess->passed_y = py;
		chess->passed_x = px;
	}
	else {
		chess->passed_x = -1;
		chess->passed_y = -1;
	}
	ADD_I_SPACE;
	ADD_I;

	// half move clock
	for (; fen[i] != ' ' && fen[i]; i++) {
		if (!isdigit(fen[i])) return 0;
		hmvc = hmvc * 10 + (unsigned char)(fen[i] - '0');
	}
	chess->hmove_clock = hmvc;
	if (!fen[i]) return 0;
	ADD_I;

	// full move number
	for (; fen[i] != ' ' && fen[i]; i++) {
		if (!isdigit(fen[i])) return 0;
		fmvn = fmvn * 10 + (unsigned char)(fen[i] - '0');
	}

	chess->hmoves += (fmvn - 1) * 2;

	return 1;
}
