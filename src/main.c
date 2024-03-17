#include "raylib.h"
#include "raymath.h"

#include <stdint.h>

const int width = 800;
const int height = 600; 

Vector2 ball_move(Vector2 *ball_center, uint32_t radius,Vector2 *ball_vel) {
	Vector2 result = *ball_center;
	if(ball_center->y + radius >= height || ball_center->y <= 0) {
		ball_vel->y = -1 * ball_vel->y;
	}
	if(ball_center->x + radius >= width || ball_center->x <= 0) {
		ball_vel->x = -1 * ball_vel->x;
	}
	result = Vector2Add(*ball_center, *ball_vel);
	return result;
}

int main() {
	// Colors(Catppuccin color pallete)
	const Color bg = {.r = 30, .g = 30, .b = 46, .a = 255};
	const Color fg = {.r = 205, .g = 214, .b = 244, .a = 255};
	const Color ball_color = {.r = 243, .g = 139, .b = 168, .a = 255};
	const Color paddle_color = {.r = 203, .g = 166, .b = 247, .a = 255};
	uint32_t ball_radius = 10;

	// Postion vectors
	Vector2 ball_center = {.x = width / 2, .y = height / 2};
	Vector2 paddle_position[2];
	paddle_position[0].x = 10;
	paddle_position[0].y = height / 2 - 40;
	paddle_position[1].x = width - 30;
	paddle_position[1].y = paddle_position[0].y;

	// Size vectors
	Vector2 paddle_size = {.x = 20, .y = 80};

	// Velocity vectors
	Vector2 ball_vel = {.x = 5, .y = 5};

	// Anti-aliasing, otherwise the shapes will have blurry outlines
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(width, height, "pong");
	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(bg);

			// Render
			DrawCircleV(ball_center, ball_radius, ball_color);
			DrawRectangleV(paddle_position[0], paddle_size, paddle_color);
			DrawRectangleV(paddle_position[1], paddle_size, paddle_color);

			// Update
			ball_center = ball_move(&ball_center, ball_radius, &ball_vel);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}