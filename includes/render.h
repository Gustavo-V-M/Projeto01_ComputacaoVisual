/* Group Members:
 * Enzo Guarnieri                               RA
 * Júlia Campolim Oste                          RA
 * Gustavo Villela Mitraud                      RA
 * Matheus Marçal Ramos de Oliveira             RA
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 *
 * Class 07N - Subject: Visual Computation - Project n. 01
 * Teacher: Andre Kishimoto
 *
 * Refferences:
 * Official SDL Wiki: https://wiki.libsdl.org/SDL3/
 * Youtube Channels:
 * https://youtube.com/playlist?list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&si=yqz1Lqy13JvwXYoc
 * https://youtube.com/playlist?list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&si=iSFdVsWFjUHksRBu
 */

#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct
{
    SDL_Window **main_window;
    SDL_Window **secondary_window;
    SDL_Renderer **main_renderer;
    SDL_Texture **unqualized_texture;
    SDL_Texture **equalized_texture;
    SDL_Renderer **secondary_renderer;
    SDL_Texture **unequalized_histogram_texture;
    SDL_Texture **equalized_histogram_texture;
    TTF_Font **font;
    char **text_histogram_texture;
    SDL_Surface *equalized_surface;
} event_loop_arguments;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    static bool sdl_init();
    static void sdl_exit();

    SDL_Window *create_window(const char *title, int width, int height, bool resizable, SDL_Window *parent = nullptr);
    void position_secondary(SDL_Window *main_window, SDL_Window *secondary_window);

    void destroy_window(SDL_Window *&window, SDL_Renderer *&renderer, SDL_Texture *&texture);

    void render(SDL_Renderer *renderer, SDL_Texture *texture);
    void render_secondary(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Window *window, TTF_Font *font, char *text);

    void event_loop(event_loop_arguments args);
    bool get_image_size(const char *path, int &width, int &height);

private:
    SDL_Rect center_button(SDL_Window *window, int button_width, int button_height, int margin);
    void draw_button(SDL_Renderer *renderer, const SDL_Rect &button_rectangle, bool hovered, bool pressed, TTF_Font *font);
    void renderText(SDL_Renderer *renderer, TTF_Font *font, int window_width, int window_height, char *text);
    static void render_texture_fit(SDL_Renderer *renderer, SDL_Texture *texture, float area_x, float area_y, float area_width, float area_height);
    bool is_closed(const SDL_Event &event, SDL_Window *window);


    bool is_button_pressed;
};

#endif // RENDER_H