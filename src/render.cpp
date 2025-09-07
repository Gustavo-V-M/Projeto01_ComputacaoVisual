#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <render.h>

void sdl_exit() {
  SDL_Log("Exiting SDL");
  SDL_Quit();
}

bool sdl_init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Fail on starting SDL: %s", SDL_GetError());
    return false;
  }

  atexit(sdl_exit);
  return true;
}

SDL_Window* create_window(const char* title, int width, int height, bool resizable, SDL_Window* parent) {
  const char* window_name = resizable ? "main" : "secondary";

  SDL_PropertiesID props = SDL_CreateProperties();
  if (props == 0) {
    SDL_Log("Error while trying to create %s window: %s", window_name, SDL_GetError());
    return NULL;
  }

  SDL_SetStringProperty (props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
  SDL_SetNumberProperty (props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
  SDL_SetNumberProperty (props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);

  if (resizable) SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, resizable);
  else SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, !resizable);

  SDL_SetNumberProperty (props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
  SDL_SetNumberProperty (props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);

  if (parent) SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_PARENT_POINTER, parent);

  SDL_Window* window = SDL_CreateWindowWithProperties(props);

  if (!window) SDL_Log("Error while creating window: %s", SDL_GetError());
  
  SDL_DestroyProperties(props);

  return window;
}

void position_secondary(SDL_Window* main_window, SDL_Window* secondary_window) {
  if (!main_window || !secondary_window) return;

  int main_x = 0, main_y = 0, main_width = 0, main_height = 0;

  SDL_GetWindowPosition(main_window, &main_x, &main_y);
  SDL_GetWindowSize(main_window, &main_width, &main_height);

  int secondary_width = 0, secondary_height = 0;

  SDL_GetWindowSize(secondary_window, &secondary_width, &secondary_height);

  int secondary_x = main_x + main_width;
  int secondary_y = main_y + (main_height - secondary_height) / 2;

  SDL_SetWindowPosition(secondary_window, secondary_x, secondary_y);
}

bool is_closed(const SDL_Event& event, SDL_Window* window) {
  return window && event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window);
}

void destroy_window(SDL_Window*& window, SDL_Renderer*& renderer, SDL_Texture*& texture) {
  if (texture) {
    SDL_DestroyTexture(texture);
    texture = NULL;
  }

  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }

  if (window) {
    SDL_DestroyWindow(window);
    window = NULL;
  }
}

void render(SDL_Renderer* renderer, SDL_Texture* texture) {
  if (!renderer || !texture) return;
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

SDL_Rect center_button(SDL_Window* window, int button_width, int button_height, int margin) {
  SDL_Rect button_rectangle {0,0,0,0};
  if (!window) return button_rectangle;

  int window_width = 0, window_height = 0;

  SDL_GetWindowSize(window, &window_width, &window_height);

  button_rectangle.w = button_width;
  button_rectangle.h = button_height;

  button_rectangle.x = (window_width - button_width) / 2;
  button_rectangle.y = window_height - button_height - margin;

  return button_rectangle;
}

void draw_button(SDL_Renderer* renderer, const SDL_Rect& button_rectangle, bool hovered, bool pressed) {
  if (!renderer || button_rectangle.w <= 0 || button_rectangle.h <= 0) return;

  Uint8 R = 70, G = 70, B = 75, A = 255;

  SDL_FRect float_rectangle {
    (float)button_rectangle.x,
    (float)button_rectangle.y,
    (float)button_rectangle.w,
    (float)button_rectangle.h
  };

  SDL_SetRenderDrawColor(renderer, R, G, B, A);
  SDL_RenderFillRect(renderer, &float_rectangle);

  SDL_SetRenderDrawColor(renderer, 180,180,190,255);
  SDL_RenderRect(renderer, &float_rectangle);
}

static void render_texture_fit(SDL_Renderer* renderer, SDL_Texture* texture, float area_x, float area_y, float area_width, float area_height) {
  if (!renderer || !texture || area_width <= 0.f || area_height <= 0.f) return;

  float image_width = 0.f, image_height = 0.f;
  SDL_GetTextureSize(texture, &image_width, &image_height);
  if (image_width <= 0.f || image_height <= 0.f) return;

  float scale_x = area_width  / image_width;
  float scale_y = area_height / image_height;
  float scale   = (scale_x < scale_y) ? scale_x : scale_y;

  float draw_w = image_width  * scale;
  float draw_h = image_height * scale;
  float draw_x = area_x + (area_width  - draw_w) * 0.5f;
  float draw_y = area_y + (area_height - draw_h) * 0.5f;
  SDL_FRect destination_rectangle{ draw_x, draw_y, draw_w, draw_h };

  SDL_RenderTexture(renderer, texture, nullptr, &destination_rectangle);
}

void render_secondary(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Window*  window) {
  if (!renderer || !window) return;

  SDL_RenderClear(renderer);

  int button_width = 200, button_height = 48, button_margin = 16, button_gap = 8;
  SDL_Rect button = center_button(window, button_width, button_height, button_margin);

  int window_width = 0, window_height = 0;

  SDL_GetWindowSize(window, &window_width, &window_height);

  float area_x = 0.f;
  float area_y = 0.f;
  float area_width = (float)window_width;
  float area_height = (float)(button.y - button_gap);
  if (area_height < 0.f) area_height = 0.f;

  if (texture) {
    render_texture_fit(renderer, texture, area_x, area_y, area_width, area_height);
  }

  draw_button(renderer, button, false, false);

  SDL_RenderPresent(renderer);
}

void event_loop(SDL_Window*& main_window, SDL_Window*& secondary_window, SDL_Renderer*& main_renderer, SDL_Texture*& main_texture, SDL_Renderer*& secondary_renderer, SDL_Texture*& secondary_texture) {

  bool running = true;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {

      if (is_closed(event, main_window) || (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))) {
        destroy_window(main_window, main_renderer, main_texture);
        destroy_window(secondary_window, secondary_renderer, secondary_texture);
        running = false;
        break;
      }
    }

    render(main_renderer, main_texture);
    render_secondary(secondary_renderer, secondary_texture, secondary_window);
  }
}

bool get_image_size(const char* path, int& width, int& height) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("IMG_Load failed (%s): %s", path, SDL_GetError());
        return false;
    }

    width = surface->w;
    height = surface->h;

    SDL_DestroySurface(surface);

    return true;
}
