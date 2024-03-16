#include "raylib.h"

int main() {
	const int width = 800;
	const int height = 600; 
	InitWindow(width, height, "pong");
	SetTargetFPS(60);
	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Hello from Raylib!", 20, 20, 64, BLACK);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}