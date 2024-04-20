#include "chess.h"

#define CHESS_MOVE(chess, x0, y0, x, y) \
	do { CHESS_AT(chess, x, y) = CHESS_AT(chess, x0, y0); \
		CHESS_AT(chess, x0, y0) = '.'; } while (0)

int is_figurine_char(int c)
{
	static char *proper = "PRNBQKprnbqk";

	for (int i = 0; proper[i]; i++)
		if ((char)c == proper[i]) return 1;

	return 0;
};

bool is_there_obstacle(const Chess *chess, int x0, int y0, int n, int sx, int sy)
{
	for (int i = 1; i < n; i++) {
		int x = x0 + i * sx;
		int y = y0 + i * sy;
		char piece = CHESS_AT(chess, x, y);
		if (is_figurine_char(piece)) return true;
	}
	return false;
}

bool is_move_legal_unsafe(const Chess *chess, int x0, int y0, int x, int y)
{
	char piece = CHESS_AT(chess, x0, y0);
	char tp = CHESS_AT(chess, x, y); // target piece

	int dx = x - x0;
	int dy = y - y0;

	if (x < 0 || x >= BOARD_COLS || y < 0 || y >= BOARD_ROWS)
		return false;

	if (!is_figurine_char(piece)) return false;

	if (is_figurine_char(tp) && (bool)islower(piece) == (bool)islower(tp))
		return false;

	if (dx == 0 && dy == 0)
		return false;

	int side = islower(piece);

	piece = piece == 'p' ? 'p' : toupper(piece);

	switch (piece) {
		case 'P':
			if (dx == 0 && dy == -1)
				return !is_figurine_char(tp);
			if (dx == 0 && dy == -2 && y0 == 6)
				return !is_figurine_char(tp) &&
					!is_figurine_char(CHESS_AT(chess, x0, y0-1));
			if (abs(dx) == 1 && dy == -1)
				return is_figurine_char(tp) ||
					x == chess->passed_x &&
					y == chess->passed_y &&
					islower(CHESS_AT(chess, chess->passed_x, chess->passed_y+1));
			return false;

		case 'p':
			if (dx == 0 && dy == 1)
				return !is_figurine_char(tp);
			if (dx == 0 && dy == 2 && y0 == 1)
				return !is_figurine_char(tp) &&
					!is_figurine_char(CHESS_AT(chess, x0, y0+1));
			if (abs(dx) == 1 && dy == 1)
				return is_figurine_char(tp) ||
					x == chess->passed_x &&
					y == chess->passed_y &&
					isupper(CHESS_AT(chess, chess->passed_x, chess->passed_y-1));
			return false;

		case 'K':
			if (dx <= 1 && dx >= -1 && dy <= 1 && dy >= -1) return true;
			if (dy != 0 || abs(dx) != 2) return false;
			int castle_avail = SGN(dx) > 0 ?
				(side ? CA_BK : CA_WK) : (side ? CA_BQ : CA_WQ);
			return chess->castle_avail & castle_avail &&
				!is_there_obstacle(chess, x0, y0, 4 - SGN(dx) > 1, SGN(dx), y0);

		case 'N':
			return abs(dx) == 2 && abs(dy) == 1 ||
				abs(dx) == 1 && abs(dy) == 2;

		case 'R':
			if (!(dx == 0 || dy == 0)) return false;
			return !is_there_obstacle(chess, x0, y0,
					abs(dx ? dx : dy), SGN(dx), SGN(dy));

		case 'B':
			if (abs(dx) != abs(dy)) return false;
			return !is_there_obstacle(chess, x0, y0,
					abs(dx ? dx : dy), SGN(dx), SGN(dy));

		case 'Q':
			if (!(dx == 0 || dy == 0 || abs(dx) == abs(dy))) return false;
			return !is_there_obstacle(chess, x0, y0,
					abs(dx ? dx : dy), SGN(dx), SGN(dy));
	}
}

bool is_king_safe(const Chess *chess, int side)
{
	char king_char = side ? 'k' : 'K';
	int king_x, king_y;
	int i;

	for (i = 0; i < BOARD_ROWS * BOARD_COLS; i++) {
		int x = i % BOARD_COLS;
		int y = i / BOARD_COLS;
		if (CHESS_AT(chess, x, y) == king_char) {
			king_x = x;
			king_y = y;
			break;
		}
	}

	if (i >= BOARD_ROWS * BOARD_COLS)
		return true;

	for (i = 0; i < BOARD_ROWS * BOARD_COLS; i++) {
		int x = i % BOARD_COLS;
		int y = i / BOARD_COLS;
		bool incheck = is_move_legal_unsafe(chess, x, y, king_x, king_y);
		if (incheck) return false;
	}

	return true;
}

bool is_move_legal(const Chess *chess, int x0, int y0, int x, int y)
{
	if (!is_move_legal_unsafe(chess, x0, y0, x, y))
		return false;

	char piece = CHESS_AT(chess, x0, y0);

	int dx = x - x0;
	int dy = y - y0;

	Chess prechess;

	if (toupper(piece) == 'K' && abs(dx) == 2) {
		if (!is_king_safe(chess, chess->hmoves % 2))
			return false;

		prechess = *chess;
		CHESS_MOVE(&prechess, x0, y0, x0+SGN(dx), y0);
		if (!is_king_safe(&prechess, prechess.hmoves % 2))
			return false;
	}

	prechess = *chess;
	CHESS_MOVE(&prechess, x0, y0, x, y);

	return is_king_safe(&prechess, prechess.hmoves % 2);
}

bool perform_move(Chess* chess, int x0, int y0, int x, int y)
{
	if (!is_move_legal(chess, x0, y0, x, y)) return false;

	char piece = CHESS_AT(chess, x0, y0);

	int dx = x - x0;
	int dy = y - y0;

	if (x == chess->passed_x && y == chess->passed_y && toupper(piece) == 'P')
		CHESS_AT(chess, chess->passed_x,
				chess->passed_y+(chess->hmoves % 2 ? -1 : 1)) = '.';

	chess->passed_x = -1;
	chess->passed_y = -1;

	if      (piece == 'k' && dx == -2) CHESS_MOVE(chess, 0, 0, x0-1, y0);
	else if (piece == 'k' && dx ==  2) CHESS_MOVE(chess, 7, 0, x0+1, y0);
	else if (piece == 'K' && dx == -2) CHESS_MOVE(chess, 0, 7, x0-1, y0);
	else if (piece == 'K' && dx ==  2) CHESS_MOVE(chess, 7, 7, x0+1, y0);
	else if (toupper(piece) == 'P' && abs(dy) == 2) {
		chess->passed_x = x;
		chess->passed_y = y0 + SGN(dy);
	}

	CHESS_MOVE(chess, x0, y0, x, y);
	chess->hmoves++;

#define CASTLE_AVAIL_CHECK(x, y, avail) \
	do { \
		if (x0 == (x) && y0 == (y) && chess->castle_avail & (avail)) \
			chess->castle_avail = chess->castle_avail & ~(avail); \
	} while (0)

	CASTLE_AVAIL_CHECK(4, 0, (CA_BQ | CA_BK));
	CASTLE_AVAIL_CHECK(4, 7, (CA_WQ | CA_WK));
	CASTLE_AVAIL_CHECK(0, 0, CA_BQ);
	CASTLE_AVAIL_CHECK(7, 0, CA_BK);
	CASTLE_AVAIL_CHECK(0, 7, CA_WQ);
	CASTLE_AVAIL_CHECK(7, 7, CA_WK);

#undef CASTLE_AVAIL_CHECK

	return true;
}
