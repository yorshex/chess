#include "chess.h"

void handle_mouse_input(Chess *chess, Rectangle board)
{
	float cell_w = board.width / BOARD_COLS;
	float cell_h = board.height / BOARD_ROWS;
	Vector2 mouse = GetMousePosition();

	if (!CheckCollisionPointRec(mouse, board)) return;

	int x = iclamp(floorf((mouse.x - board.x) / cell_w), 0, BOARD_COLS - 1);
	int y = iclamp(floorf((mouse.y - board.y) / cell_h), 0, BOARD_ROWS - 1);

	if (chess->sel_x < 0 || chess->sel_y < 0) {
		char piece = CHESS_AT(chess, x, y);

		if (!is_figurine_char(piece)) return;
		if (chess->hmoves % 2 == 0 && !isupper(piece)) return;
		if (chess->hmoves % 2 == 1 && !islower(piece)) return;

		chess->sel_x = x;
		chess->sel_y = y;

		return;
	}

	if (chess->sel_x != x || chess->sel_y != y)
		perform_move(chess, chess->sel_x, chess->sel_y, x, y);

	chess->sel_x = -1;
	chess->sel_y = -1;
}
