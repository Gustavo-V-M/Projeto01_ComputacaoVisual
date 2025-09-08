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

#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

// Converts an SDL_Surface to greyscale in-place
void to_grey_scale(SDL_Surface* surface) {
    if (!surface) return;

    SDL_LockSurface(surface);

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    
    Uint8 r, g, b, a;
    Uint32* pixels = (Uint32*)surface->pixels;
    int pixel_count = (surface->w) * (surface->h);

    for (int i = 0; i < pixel_count; ++i) {
        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
        Uint8 grey = (Uint8)(0.2125 * r + 0.7154 * g + 0.0721 * b);
        pixels[i] = SDL_MapRGBA(format, NULL, grey, grey, grey, a);
    }

    SDL_UnlockSurface(surface);
}
