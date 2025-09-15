#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <render.h>

Renderer::Renderer() {
  is_button_pressed = false;
}

Renderer::~Renderer()
{
  sdl_exit();
}

void Renderer::sdl_exit()
{
  SDL_Log("Exiting SDL");
  SDL_Quit();
}

bool Renderer::sdl_init()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Fail on starting SDL: %s", SDL_GetError());
    return false;
  }
  return true;
}

SDL_Window *Renderer::create_window(const char *title, int width, int height, bool resizable, SDL_Window *parent)
{
  const char *window_name = resizable ? "main" : "secondary";
  SDL_PropertiesID props = SDL_CreateProperties();
  if (props == 0)
  {
    SDL_Log("Error while trying to create %s window: %s", window_name, SDL_GetError());
    return NULL;
  }
  SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title);
  SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
  SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);
  if (resizable)
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, resizable);
  else
    SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, !resizable);
  SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED);
  SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED);
  if (parent)
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_PARENT_POINTER, parent);
  SDL_Window *window = SDL_CreateWindowWithProperties(props);
  if (!window)
    SDL_Log("Error while creating window: %s", SDL_GetError());
  SDL_DestroyProperties(props);
  return window;
}

void Renderer::position_secondary(SDL_Window *main_window, SDL_Window *secondary_window)
{
  if (!main_window || !secondary_window)
    return;
  int main_x = 0, main_y = 0, main_width = 0, main_height = 0;
  SDL_GetWindowPosition(main_window, &main_x, &main_y);
  SDL_GetWindowSize(main_window, &main_width, &main_height);
  int secondary_width = 0, secondary_height = 0;
  SDL_GetWindowSize(secondary_window, &secondary_width, &secondary_height);
  int secondary_x = main_x + main_width;
  int secondary_y = main_y + (main_height - secondary_height) / 2;
  SDL_SetWindowPosition(secondary_window, secondary_x, secondary_y);
}

bool Renderer::is_closed(const SDL_Event &event, SDL_Window *window)
{
  return window && event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window);
}

void Renderer::destroy_window(SDL_Window *&window, SDL_Renderer *&renderer, SDL_Texture *&texture)
{
  if (texture)
  {
    SDL_DestroyTexture(texture);
    texture = NULL;
  }
  if (renderer)
  {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }
  if (window)
  {
    SDL_DestroyWindow(window);
    window = NULL;
  }
}

void Renderer::render(SDL_Renderer *renderer, SDL_Texture *texture)
{
  if (!renderer || !texture)
    return;
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
SDL_Rect Renderer::center_button(SDL_Window *window, int button_width, int button_height, int margin) {
    SDL_Rect rect{0, 0, 0, 0};
    if (!window) return rect;

    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    rect.w = button_width;
    rect.h = button_height;
    rect.x = (window_width - button_width) / 2;
    rect.y = window_height - button_height - margin;

    return rect;
}

SDL_Rect Renderer::side_button(SDL_Window *window, int button_width, int button_height, int margin) {
    SDL_Rect rect{0, 0, 0, 0};
    if (!window) return rect;

    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    rect.w = button_width;
    rect.h = button_height;
    rect.x = window_width - button_width - 10;
    rect.y = window_height - button_height - margin;

    return rect;
}

void Renderer::draw_button(SDL_Renderer *renderer, Button& button) {
    if (!renderer || button.rect.w <= 0 || button.rect.h <= 0) return;

    SDL_FRect float_rect{
        (float)button.rect.x,
        (float)button.rect.y,
        (float)button.rect.w,
        (float)button.rect.h
    };

    // Base
    SDL_SetRenderDrawColor(renderer, 70, 70, 75, 255);
    SDL_RenderFillRect(renderer, &float_rect);

    // Hover
    if (button.hovered) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        SDL_RenderFillRect(renderer, &float_rect);
    }

    // Pressed
    if (button.pressed) {
        static Uint32 last_toggle_time = 0;
        Uint32 current_time = SDL_GetTicks();
        const Uint32 debounce_delay = 600;

        if (current_time - last_toggle_time > debounce_delay) {
            if (button.on_click) button.on_click(); 
            last_toggle_time = current_time;
        }
    }


    // Borda
    SDL_SetRenderDrawColor(renderer, 180, 180, 190, 255);
    SDL_RenderRect(renderer, &float_rect);
}


void Renderer::handle_event(const SDL_Event& e, std::vector<Button>& buttons) {
    SDL_Point mousePoint;

    int mx, my;

    switch (e.type) {
        case SDL_EVENT_MOUSE_MOTION:
            mx = e.motion.x;
            my = e.motion.y;
            mousePoint = { mx, my };
            for (auto& btn : buttons) {
                btn.hovered = SDL_PointInRect(&mousePoint, &btn.rect);
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (e.button.button == SDL_BUTTON_LEFT) {
                mx = e.button.x;
                my = e.button.y;
                mousePoint = { mx, my };
                for (auto& btn : buttons) {
                    if (SDL_PointInRect(&mousePoint, &btn.rect)) {
                        btn.pressed = true;

                        if (btn.id == 1) {
                            SDL_Log("Botão 1 pressionado!");
                            is_button_pressed = !is_button_pressed;
                        } else if (btn.id == 2) {
                            SDL_Log("Botão 2 pressionado!"); 
                        }
                    }
                }
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (e.button.button == SDL_BUTTON_LEFT) {
                // Reseta todos os botões quando soltar o botão do mouse
                for (auto& btn : buttons) {
                    btn.pressed = false;
                }
            }
            break;
    }
}


void Renderer::render_texture_fit(SDL_Renderer *renderer, SDL_Texture *texture, float area_x, float area_y, float area_width, float area_height)
{
  if (!renderer || !texture || area_width <= 0.f || area_height <= 0.f)
    return;
  float image_width = 0.f, image_height = 0.f;
  SDL_GetTextureSize(texture, &image_width, &image_height);
  if (image_width <= 0.f || image_height <= 0.f)
    return;
  float scale_x = area_width / image_width;
  float scale_y = area_height / image_height;
  float scale = (scale_x < scale_y) ? scale_x : scale_y;
  float draw_w = image_width * scale;
  float draw_h = image_height * scale;
  float draw_x = area_x + (area_width - draw_w) * 0.5f;
  float draw_y = area_y + (area_height - draw_h) * 0.5f;
  SDL_FRect destination_rectangle{draw_x, draw_y, draw_w, draw_h};
  SDL_RenderTexture(renderer, texture, nullptr, &destination_rectangle);
}
void Renderer::render_secondary(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Window *window)
{
    if (!renderer || !window)
        return;

    SDL_RenderClear(renderer);

    int button_width = 200;
    int button_height = 48;
    int button_margin = 16;
    int button_gap = 32;

    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    float area_x = 0.f;
    float area_y = 0.f;
    float area_width = (float)window_width;
    float area_height = (float)(window_height - button_height - button_margin * 2);
    if (area_height < 0.f)
        area_height = 0.f;

    if (texture)
    {
        render_texture_fit(renderer, texture, area_x, area_y, area_width, area_height);
    }

    int total_width = (button_width * 2) + button_gap;
    int start_x = (window_width - total_width) / 2;
    int y = window_height - button_height - button_margin;

    Button button1{ { start_x, y, button_width, button_height }, 1, false, false, "Botão 1" };
    Button button2{ { start_x + button_width + button_gap, y, button_width, button_height }, 2, false, false, "Botão 2" };

    // Mouse
    float mouse_x, mouse_y;
    Uint32 mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Point mouse_point{ (int) mouse_x, (int) mouse_y };

    // Atualizar estados
    button1.hovered = SDL_PointInRect(&mouse_point, &button1.rect);
    button1.pressed = button1.hovered && (mouse_buttons & SDL_BUTTON_LMASK);

    button2.hovered = SDL_PointInRect(&mouse_point, &button2.rect);
    button2.pressed = button2.hovered && (mouse_buttons & SDL_BUTTON_LMASK);

    draw_button(renderer, button1);
    draw_button(renderer, button2);

    secondary_buttons.push_back(button1);
    secondary_buttons.push_back(button2);

    SDL_RenderPresent(renderer);
}

void Renderer::event_loop(event_loop_arguments args)
{
    SDL_Log("Entering event loop");
    bool running = true;  
    SDL_Event event;

    while (running)
    {
        // Tratamento de eventos
        while (SDL_PollEvent(&event))
        {
            // Fechar janelas ou pressionar ESC
            if (is_closed(event, *args.main_window) || 
                event.type == SDL_EVENT_QUIT || 
                (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))
            {
                destroy_window(*args.main_window, *args.main_renderer, *args.unqualized_texture);
                destroy_window(*args.secondary_window, *args.secondary_renderer, *args.unequalized_histogram_texture);
                running = false;
                break;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
              if (event.key.key == SDLK_S) {
                  SDL_Log("Tecla S pressionada! Salvando imagem...");

                  SDL_Surface* surface_to_save = *args.equalized_surface;
                  if (!surface_to_save) {
                      SDL_Log("Erro: equalized_surface é NULL");
                  } else {
                      if (IMG_SavePNG(surface_to_save, "output_image.png") != 0) {
                          SDL_Log("Erro ao salvar imagem: %s", SDL_GetError());
                      } else {
                          SDL_Log("Imagem salva com sucesso como output_image.png!");
                      }
                  }
            }
}

            // Handle os botões da janela secundária
            handle_event(event, secondary_buttons);
        }

        // Render da janela principal
        if (!is_button_pressed)
        {
            render(*args.main_renderer, *args.unqualized_texture);
        }
        else
        {
            render(*args.main_renderer, *args.equalized_texture);
        }

        // Render da janela secundária (botões inclusos)
        render_secondary(*args.secondary_renderer, *args.unequalized_histogram_texture, *args.secondary_window);

        // Desenha os botões da janela secundária
        for (auto& btn : secondary_buttons)
        {
            draw_button(*args.secondary_renderer, btn);
        }
    }
}


bool Renderer::get_image_size(const char *path, int &width, int &height)
{
  SDL_Surface *surface = IMG_Load(path);
  if (!surface)
  {
    SDL_Log("IMG_Load failed (%s): %s", path, SDL_GetError());
    return false;
  }
  width = surface->w;
  height = surface->h;
  SDL_DestroySurface(surface);
  return true;
}