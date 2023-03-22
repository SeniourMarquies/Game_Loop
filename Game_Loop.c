
#include "./constants.h"
#include <SDL.h>
#include <stdio.h>

int last_frame_time = 0;
int game_is_running = 0;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;

int Initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL window.\n");
    return FALSE;
  }
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating window.\n");
    return FALSE;
  }
  renderer = SDL_CreateRenderer(
      window, -1,
      0); // -1 is for driver code and 0 for special flag which I do not care.
  if (!renderer) {
    fprintf(stderr, "Error creating renderer. \n");
    return FALSE;
  }
  return TRUE;
}
void setup() {
  ball.x = 20;
  ball.y = 20;
  ball.width = 50;
  ball.height = 50;
}
void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    game_is_running = FALSE;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      game_is_running = FALSE;
    break;
  default:
    break;
  }
}
void update() {

  // Waste some time / sleep untill we reach the frame target time
  // this is manual implementation and this consumes lots of cpu so we gonna use
  // sdl_delay function.
  // while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time +
  // FRAME_TARGET_TIME));

  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
  // only call this delay if we are too fast to process this frame
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    SDL_Delay(time_to_wait);
  ball.x += 50 * delta_time;
  ball.y += 50 * delta_time;
}
void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  //  Here is where we can start drawing our gameobjects.
  // Draw a rectangle
  SDL_Rect ball_rect = {ball.x, ball.y, ball.width, ball.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);
  SDL_RenderPresent(renderer); // Swapping buffer. Back-Fort buffer.
}
void destroy_window() {

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char *args[]) {
  game_is_running = Initialize_window();
  setup();

  while (game_is_running) {
    process_input();
    update();
    render();
  }
  destroy_window();
  return 0;
}