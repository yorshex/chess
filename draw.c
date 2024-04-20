#include "chess.h"

void draw_chessboard(const Chess *chess, Rectangle dest, Texture chessmen)
{
	float cell_w = dest.width / BOARD_COLS;
	float cell_h = dest.height / BOARD_ROWS;

	for (int y = 0; y < BOARD_ROWS; y++)
		for (int x = 0; x < BOARD_COLS; x++) {
			Rectangle rect = {
				cell_w * x + dest.x,
				cell_h * y + dest.y,
				ceilf(cell_w),
				ceilf(cell_h),
			};

			DrawRectangleRec(rect, (x + y) % 2 ? GRAY : LIGHTGRAY);

			char piece = CHESS_AT(chess, x, y);
			int piece_index = get_piece_index(piece);

			Rectangle source = {
				0,
				chessmen.width * piece_index,
				chessmen.width,
				chessmen.width,
			};

			if (piece_index >= 0)
				DrawTexturePro(chessmen, source, rect,
						(Vector2){0}, 0.0, isupper(piece) ? WHITE : BLACK);
		}

	if (chess->sel_x >= 0 && chess->sel_y >= 0) {
		float thick = roundf(fminf(cell_w, cell_h) * 0.1);
		Rectangle rect = {
				cell_w * chess->sel_x + dest.x - thick,
				cell_h * chess->sel_y + dest.y - thick,
				ceilf(cell_w)+thick*2,
				ceilf(cell_h)+thick*2,
		};
		DrawRectangleLinesEx(rect, thick, ORANGE);
	}
}
