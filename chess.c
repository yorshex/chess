#include "chess.h"

static const char *CHESS_INIT_FEN =
	"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Rectangle get_chessboard_rect(void)
{
	Vector2 wsz = {GetScreenWidth(), GetScreenHeight()};

	float mn_side = fminf(wsz.x, wsz.y);
	Vector2 off = {0};

	if (wsz.x > wsz.y)
		off.x = (wsz.x - mn_side) * .5;
	else
		off.y = (wsz.y - mn_side) * .5;

	return (Rectangle){
		0 + mn_side * 0.1 + off.x,
		0 + mn_side * 0.1 + off.y,
		mn_side * 0.8,
		mn_side * 0.8,
	};
}

Rectangle get_promotion_rect(void)
{
	Rectangle board = get_chessboard_rect();
	float cell_w = board.width * (1.0 / BOARD_COLS);
	float cell_h = board.height * (1.0 / BOARD_ROWS);

	return (Rectangle){
		board.x + cell_w * 2.0,
		board.y + cell_h * 3.5,
		cell_w * 4.0,
		cell_h,
	};
}

int main(void)
{
	Chess chess_init = {
		.sel_x = -1, .sel_y = -1,
		.prom_x = -1, .prom_y = -1
	};
	fen_to_chess(CHESS_INIT_FEN, &chess_init);

	Chess chess = chess_init;

	SetTraceLogLevel(LOG_WARNING);

	InitWindow(800, 600, "Chess");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Rectangle chessboard_rect = get_chessboard_rect();
	Rectangle promotion_rect = get_promotion_rect();
	Texture chessmen_texture = LoadTexture("res/chessmen.png");

	while (!WindowShouldClose()) {
		if (IsWindowResized()) {
			chessboard_rect = get_chessboard_rect();
			promotion_rect = get_promotion_rect();
		}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			if (chess.prom_x >= 0 && chess.prom_y >= 0)
				handle_lmb_promotion(&chess, promotion_rect);
			else
				handle_lmb_chessboard(&chess, chessboard_rect);
		}

		BeginDrawing();

		ClearBackground(WHITE);

		draw_chessboard(&chess, chessboard_rect, chessmen_texture);
		if (chess.prom_x >= 0 && chess.prom_y >= 0) {
			DrawRectangle(0, 0,
					GetScreenWidth(), GetScreenHeight(),
					(Color){0xFF, 0xFF, 0xFF, 0x7F});
			draw_promotion_ui(&chess, promotion_rect, chessmen_texture);
		}

		EndDrawing();
	}

	UnloadTexture(chessmen_texture);
	CloseWindow();

	return 0;
}
