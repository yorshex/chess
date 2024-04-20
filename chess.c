#include "chess.h"

Rectangle get_chessboard_rect()
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

int main(void)
{
	Chess chess_init = {.sel_x = -1, .sel_y = -1};
	fen_to_chess(CHESS_INIT_FEN, &chess_init);

	Chess chess = chess_init;

	InitWindow(800, 600, "Chess");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	Rectangle chessboard_rect = get_chessboard_rect();
	Texture chessmen_texture = LoadTexture("res/chessmen.png");

	while (!WindowShouldClose()) {
		if (IsWindowResized())
			chessboard_rect = get_chessboard_rect();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			handle_mouse_input(&chess, chessboard_rect);

		BeginDrawing();

		ClearBackground(WHITE);

		draw_chessboard(&chess, chessboard_rect, chessmen_texture);

		EndDrawing();
	}

	UnloadTexture(chessmen_texture);
	CloseWindow();

	return 0;
}
