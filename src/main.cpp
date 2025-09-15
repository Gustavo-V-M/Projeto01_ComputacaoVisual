/* Group Members:
 * Enzo Guarnieri                               RA
 * Júlia Campolim Oste                          RA
 * Gustavo Villela Mitraud                      RA
 * Matheus Marçal Ramos de Oliveira             RA
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 *
 * Class 07N - Subject: Visual Computing - Project n. 01
 * Teacher: Andre Kishimoto
 *
 * Refferences:
 * Official SDL Wiki: https://wiki.libsdl.org/SDL3/
 * Official SDL_ttf Documentation: https://www.libsdl.org/projects/old/SDL_ttf/docs/SDL_ttf.html
 * Book:
 * GONZALEZ, R. C.; WOODS, R. E. Processamento digital de imagens, 3ª edição.
 * Youtube Channels:
 * https://youtube.com/playlist?list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&si=yqz1Lqy13JvwXYoc
 * https://youtube.com/playlist?list=PLvv0ScY6vfd-RZSmGbLkZvkgec6lJ0BfX&si=iSFdVsWFjUHksRBu
 * Font:
 * https://fonts.google.com/specimen/Roboto?preview.text=Whereas%20recognition%20of%20the%20inherent%20dignity
 */

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <histogram.h>
#include <render.h>
#include <to_grey_scale.h>
#include <equalizator.h>

using namespace std;

int main(int argc, char **argv)
{
    Renderer renderer;

    // Starts SDL
    if (!renderer.sdl_init())
        return EXIT_FAILURE;

    atexit(renderer.sdl_exit);

    // Images paths
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return EXIT_FAILURE;
    }
    const char *main_image_path = argv[1];

    // Images sizes
    int main_width, main_height;
    int secondary_width = 500, secondary_height = 500;

    // Gets main image dimensions
    if (!renderer.get_image_size(main_image_path, main_width, main_height)) {
        return EXIT_FAILURE;
    }

    // Creates main and secondary windows
    SDL_Window *main_window = renderer.create_window("Main Window", main_width, main_height, true, NULL);
    SDL_Window *secondary_window = main_window ? renderer.create_window("Secondary Window", secondary_width, secondary_height, false, main_window) : NULL;

    // Checks if both windows were created
    if (!main_window && !secondary_window)
    {
        SDL_Log("No window was created.");
        return EXIT_FAILURE;
    }

    // Position secondary window at the right center side of the main window
    if (main_window && secondary_window)
        renderer.position_secondary(main_window, secondary_window);

    // Renders both images on their respective windows
    SDL_Renderer *main_renderer = main_window ? SDL_CreateRenderer(main_window, NULL) : NULL;
    SDL_Renderer *secondary_renderer = secondary_window ? SDL_CreateRenderer(secondary_window, NULL) : NULL;

    SDL_Surface *main_surface = IMG_Load(main_image_path);

    if (!main_surface)
    {
        SDL_Log("IMG_Load failed (%s): %s", main_image_path, SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface *converted_surface = SDL_ConvertSurface(main_surface, SDL_PIXELFORMAT_RGBA32);
    
    if (!converted_surface)
    {
        SDL_Log("SDL_ConvertSurface failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("../../assets/font-roboto.ttf", 18.0f);
    //TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    if (!font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(main_surface);
    main_surface = converted_surface;

    to_grey_scale(main_surface);

    Histogram histogram(main_surface);
    Equalizator equalizator(main_surface, &histogram);

    SDL_Texture *main_texture_grey = main_renderer ? SDL_CreateTextureFromSurface(main_renderer, main_surface) : NULL;
    if (!main_texture_grey)
    {
        SDL_Log("SDL_CreateTextureFromSurface (main) failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface *secundary_suface = histogram.render_histogram(secondary_renderer, secondary_width, secondary_height);
    SDL_Texture *secondary_texture = secondary_renderer ? SDL_CreateTextureFromSurface(secondary_renderer, secundary_suface) : NULL;
    SDL_DestroySurface(secundary_suface);

    // Checks if the images were correctly displayed
    if (!main_texture_grey)
        SDL_Log("IMG_LoadTexture (main) failed: %s", SDL_GetError());

    if (!secondary_texture)
        SDL_Log("IMG_LoadTexture (secondary) failed: %s", SDL_GetError());

    SDL_Surface *equalized_surface = equalizator.get_equalized_surface();
    Histogram *equalized_histogram = equalizator.get_equalized_histogram();
    SDL_Surface *equalized_histogram_surface = equalized_histogram->render_histogram(secondary_renderer, secondary_width, secondary_height);

    SDL_Texture *equalized_texture = main_renderer ? SDL_CreateTextureFromSurface(main_renderer, equalized_surface) : NULL;
    SDL_Texture *equalized_histogram_texture = secondary_renderer ? SDL_CreateTextureFromSurface(secondary_renderer, equalized_histogram_surface) : NULL;

    float histogram_mean = histogram.mean_intensity();
    float histogram_stddev = histogram.standard_deviation();

    const char *intensity = histogram_mean < 85 ? "escura" : (histogram_mean > 170 ? "clara" : "média");
    const char *contrast = histogram_stddev < 35 ? "baixo" : (histogram_stddev > 80 ? "alto" : "médio");

    auto text = new char[80];
    snprintf(text, 80, "Imagem %s de %s contraste", intensity, contrast);

    // Calls the event loop
    event_loop_arguments args;
    args.main_window = &main_window;
    args.secondary_window = &secondary_window;
    args.main_renderer = &main_renderer;
    args.unqualized_texture = &main_texture_grey;
    args.secondary_renderer = &secondary_renderer;
    args.unequalized_histogram_texture = &secondary_texture;
    args.equalized_histogram_texture = &equalized_histogram_texture;
    args.equalized_texture = &equalized_texture;
    args.font = &font;
    args.text_histogram_texture = &text;

    renderer.event_loop(args);

    return EXIT_SUCCESS;
}