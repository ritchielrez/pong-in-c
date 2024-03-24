#include <stdint.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

const uint32_t width = 800;
const uint32_t height = 600;

const uint32_t wall_padding = 10;

const uint32_t max_score = 3;

bool gameOver = false;
bool ballMoving = false;

Vector2 ball_center, ball_vel, player_pos[2], player_size;
int32_t player_vel_y[2];
uint32_t ball_radius = 10, player_score[2];
uint8_t winner_id;

void init() {
  ball_center.x = width / 2;
  ball_center.y = height / 2;

  player_size.x = 20;
  player_size.y = 80;

  // In the game, the 0th paddle is the paddle that is controlled by the user,
  // and the 1st paddle is the paddle which is uncontrollable.
  player_pos[0].x = wall_padding;
  player_pos[0].y = height / 2 - (player_size.y / 2);
  player_pos[1].x = width - (player_size.x + wall_padding);
  player_pos[1].y = player_pos[0].y;

  ball_vel.x = 5;
  ball_vel.y = 5;
  player_vel_y[0] = 8;
  player_vel_y[1] = 8;
}

void ball_move() {
  if ((ball_center.y + ball_radius + ball_vel.y >= height && ball_vel.y > 0) ||
      (ball_center.y + ball_vel.y <= 0 && ball_vel.y < 0)) {
    ball_vel.y *= -1;
  }
  if ((ball_center.x + ball_radius + ball_vel.x >= width - wall_padding &&
       ball_vel.x > 0)) {
    player_score[0]++;
    ballMoving = false;
    init();
  } else if ((ball_center.x + ball_vel.x <= 0 + wall_padding &&
              ball_vel.x < 0)) {
    player_score[1]++;
    ballMoving = false;
    init();
  }
  ball_center = Vector2Add(ball_center, ball_vel);
}

bool aabb_vs_circle_collision(Vector2 box_pos, Vector2 box_size,
                              Vector2 circle_center, uint32_t circle_radius) {
  return ((box_pos.x + box_size.x >= circle_center.x - circle_radius) &&
          (box_pos.x <= circle_center.x + circle_radius) &&
          (box_pos.y + box_size.y >= circle_center.y - circle_radius) &&
          (box_pos.y <= circle_center.y + circle_radius));
}

int main() {
  // Colors(Catppuccin color pallete)
  const Color bg = {.r = 30, .g = 30, .b = 46, .a = 255};
  const Color fg = {.r = 205, .g = 214, .b = 244, .a = 255};
  const Color ball_color = {.r = 243, .g = 139, .b = 168, .a = 255};
  const Color paddle_color = {.r = 203, .g = 166, .b = 247, .a = 255};

  // Anti-aliasing, otherwise the shapes will have blurry outlines
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(width, height, "pong");
  SetTargetFPS(60);

  init();
  player_score[0] = 0;
  player_score[1] = 0;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(bg);

    // Render
    if (gameOver) {
      const char *win_msg = TextFormat("Player %d won", winner_id + 1);
      DrawText(win_msg, (width / 2) - (MeasureText(win_msg, 60) / 2),
               height / 2 - 60, 60, fg);
      if (IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        init();
        player_score[0] = 0;
        player_score[1] = 0;
        continue;
      }
    } else {
      DrawCircleV(ball_center, ball_radius, ball_color);
      DrawRectangleV(player_pos[0], player_size, paddle_color);
      DrawRectangleV(player_pos[1], player_size, paddle_color);
      DrawText(TextFormat("%02d", player_score[0]), width * 1 / 4 - 30, 0, 60,
               fg);
      DrawText(TextFormat("%02d", player_score[1]), width * 3 / 4 + 30, 0, 60,
               fg);
    }

    // Update
    if (ballMoving) {
      ball_move();
    } else if (IsKeyPressed(KEY_ENTER)) {
      ballMoving = true;
      ball_move();
    }

    // Player 0 movement
    if (IsKeyDown(KEY_W) && player_pos[0].y > 0) {
      player_pos[0].y -= player_vel_y[0];
    }
    if (IsKeyDown(KEY_S) && player_pos[0].y + player_size.y < height) {
      player_pos[0].y += player_vel_y[0];
    }

    if (IsKeyDown(KEY_UP) && player_pos[1].y > 0) {
      player_pos[1].y -= player_vel_y[1];
    }
    if (IsKeyDown(KEY_DOWN) && player_pos[1].y + player_size.y < height) {
      player_pos[1].y += player_vel_y[1];
    }

    // Collision detection between paddles and the ball
    if (aabb_vs_circle_collision(player_pos[0], player_size, ball_center,
                                 ball_radius)) {
      ball_vel.x = fabsf(ball_vel.x);
      ball_vel.y = ball_vel.y;
    } else if (aabb_vs_circle_collision(player_pos[1], player_size, ball_center,
                                        ball_radius)) {
      ball_vel.x = -(fabsf(ball_vel.x));
      ball_vel.y = ball_vel.y;
    }

    // Check the player's scores
    if (player_score[0] == max_score) {
      winner_id = 0;
      gameOver = true;
    } else if (player_score[1] == max_score) {
      winner_id = 1;
      gameOver = true;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
