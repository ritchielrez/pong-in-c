#include <stdint.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

const uint32_t width = 800;
const uint32_t height = 600;

const uint32_t wall_padding = 10;

const uint32_t max_score = 3;

bool ball_moving = false;

typedef enum {
  screen_start,
  screen_gameplay,
  screen_over,
} GameScreen;

typedef enum { button_normal, button_hover, button_pressed } ButtonState;

typedef struct {
  Rectangle rect;
  ButtonState state;
  const char *text;
  uint32_t text_size;
} Button;

Vector2 ball_center, ball_vel, player_pos[2], player_size;
int32_t player_vel_y[2];
uint32_t ball_radius = 10, player_score[2];
uint8_t winner_id;
GameScreen game_screen = screen_start;

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
    ball_moving = false;
    init();
  } else if ((ball_center.x + ball_vel.x <= 0 + wall_padding &&
              ball_vel.x < 0)) {
    player_score[1]++;
    ball_moving = false;
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

bool aabb_vs_point_collision(Rectangle rect, Vector2 point) {
  return rect.x + rect.width > point.x && rect.x < point.x &&
         rect.y + rect.height > point.y && rect.y < point.y;
}

Button button_begin(Rectangle *rect, const char *text, uint32_t text_size) {
  Button button = {
      .rect = *rect,
      .state = button_normal,
      .text = text,
      .text_size = text_size,
  };

  float text_width = MeasureText(button.text, button.text_size);
  float text_height = button.text_size;
  Color text_fg;

  Vector2 mouse_pos = GetMousePosition();
  if (aabb_vs_point_collision(button.rect, mouse_pos)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      button.state = button_pressed;
    } else {
      button.state = button_hover;
    }
    DrawRectangleRounded(button.rect, 10, 10, btn_border);
    text_fg = bg;
  } else {
    DrawRectangleRoundedLines(button.rect, 10, 10, 2, btn_border);
    text_fg = fg;
  }
  DrawText(button.text,
           (button.rect.x + (button.rect.width / 2)) - (text_width / 2),
           (button.rect.y + (button.rect.height / 2)) - (text_height / 2),
           text_height, text_fg);
  return button;
}

int main() {
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

    switch (game_screen) {
      case screen_start: {
        DrawText("Pong", width / 2 - (MeasureText("Pong", 60) / 2), 20, 60, fg);

        Rectangle rect = {.x = 0, .y = 0, .width = 150, .height = 60};
        rect.x = (width / 2) - (rect.width / 2);
        rect.y = (height / 2) - (rect.height / 2);
        Button start_btn = button_begin(&rect, "Play", 30);

        if (start_btn.state == button_pressed) {
          game_screen = screen_gameplay;
        }

        break;
      }
      case screen_gameplay: {
        DrawCircleV(ball_center, ball_radius, ball_color);
        DrawRectangleV(player_pos[0], player_size, paddle_color);
        DrawRectangleV(player_pos[1], player_size, paddle_color);
        DrawText(TextFormat("%02d", player_score[0]),
                 width * 1 / 4 -
                     (MeasureText(TextFormat("%02d", player_score[0]), 60) / 2),
                 0, 60, fg);
        DrawText(TextFormat("%02d", player_score[1]),
                 width * 3 / 4 -
                     (MeasureText(TextFormat("%02d", player_score[0]), 60) / 2),
                 0, 60, fg);
        if (ball_moving) {
          ball_move();
        } else if (IsKeyPressed(KEY_SPACE)) {
          ball_moving = true;
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
        } else if (aabb_vs_circle_collision(player_pos[1], player_size,
                                            ball_center, ball_radius)) {
          ball_vel.x = -(fabsf(ball_vel.x));
          ball_vel.y = ball_vel.y;
        }

        // Check the player's scores
        if (player_score[0] == max_score) {
          winner_id = 0;
          game_screen = screen_over;
        } else if (player_score[1] == max_score) {
          winner_id = 1;
          game_screen = screen_over;
        }

        break;
      }
      case screen_over: {
        const char *win_msg = TextFormat("Player %d won", winner_id + 1);
        const char *restart_msg = "Press ENTER to restart";
        DrawText(win_msg, (width / 2) - (MeasureText(win_msg, 60) / 2), 20, 60,
                 fg);

        Rectangle rect = {.x = 0, .y = 0, .width = 160, .height = 60};
        rect.x = (width / 2) - (rect.width / 2);
        rect.y = (height / 2) - (rect.height / 2);
        Button restart_btn = button_begin(&rect, "Restart", 30);

        if (restart_btn.state == button_pressed) {
          game_screen = screen_gameplay;
          init();
          player_score[0] = 0;
          player_score[1] = 0;
        }

        break;
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
