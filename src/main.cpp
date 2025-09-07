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


#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <histogram.h>
#include <render.h>
#include <to_grey_scale.h>


int main(int argc, char **argv)
{
  // Starts SDL
  if (!sdl_init()) return EXIT_FAILURE;

  // Images paths
  const char* main_image_path = "assets/image.jpg";
  const char* secondary_image_path  = "assets/image.jpg";

  // Images sizes
  int main_width, main_height;
  int secondary_width = 500, secondary_height = 500;

  // Gets main image dimensions
  get_image_size(main_image_path, main_width, main_height);

  // Creates main and secondary windows
  SDL_Window* main_window = create_window("Main Window", main_width, main_height, true, NULL);
  SDL_Window* secondary_window = main_window ? create_window("Secondary Window", secondary_width, secondary_height, false, main_window) : NULL;

  // Checks if both windows were created
  if (!main_window && !secondary_window) {
    SDL_Log("No window was created.");
    return EXIT_FAILURE;
  }

  // Position secondary window at the right center side of the main window
  if (main_window && secondary_window) position_secondary(main_window, secondary_window);
  
  // Renders both images on their respective windows
  SDL_Renderer* main_renderer = main_window ? SDL_CreateRenderer(main_window, NULL) : NULL;
  SDL_Renderer* secondary_renderer = secondary_window ? SDL_CreateRenderer(secondary_window, NULL) : NULL;

  SDL_Surface* main_surface = IMG_Load(main_image_path);
  if (!main_surface) {
    SDL_Log("IMG_Load failed (%s): %s", main_image_path, SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Surface* converted_surface = SDL_ConvertSurface(main_surface, SDL_PIXELFORMAT_RGBA32);
  if (!converted_surface) {
    SDL_Log("SDL_ConvertSurface failed: %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_DestroySurface(main_surface);
  main_surface = converted_surface;
  to_grey_scale(main_surface);


  SDL_Texture* main_texture_grey = main_renderer ? SDL_CreateTextureFromSurface(main_renderer, main_surface) : NULL;
  if (!main_texture_grey) {
    SDL_Log("SDL_CreateTextureFromSurface (main) failed: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  Histogram histogram(main_surface);
  if (histogram.is_dark()) {
    SDL_Log("The image is dark.");
  } else {
    SDL_Log("The image is not dark.");
  }

  if (histogram.is_high_contrast()) {
    SDL_Log("The image is high contrast.");
  } else {
    SDL_Log("The image is not high contrast.");
  }
  // SDL_Texture* main_texture = main_renderer ? IMG_LoadTexture(main_renderer, main_image_path) : NULL;
  SDL_Surface* secundary_suface = histogram.render_histogram(secondary_renderer, secondary_width, secondary_height);
  SDL_Texture* secondary_texture = secondary_renderer ? SDL_CreateTextureFromSurface(secondary_renderer, secundary_suface) : NULL;
  SDL_DestroySurface(secundary_suface);

  // Checks if the images were correctly displayed
  if (!main_texture_grey) SDL_Log("IMG_LoadTexture (main) failed: %s", SDL_GetError());
  if (!secondary_texture) SDL_Log("IMG_LoadTexture (secondary) failed: %s", SDL_GetError());




  // Calls the event loop
  event_loop(main_window, secondary_window, main_renderer, main_texture_grey, secondary_renderer, secondary_texture);

  return EXIT_SUCCESS;
}