/* Group Members:
 * Enzo Guarnieri                               RA 10410074
 * Júlia Campolim Oste                          RA 10408802
 * Gustavo Villela Mitraud                      RA 10400866
 * Matheus Marçal Ramos de Oliveira             RA 10409001
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
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
    const int pixel_count = surface->w * surface->h;

    for (int i = 0; i < pixel_count; ++i) {
        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
        Uint8 grey = (Uint8)(0.2125 * r + 0.7154 * g + 0.0721 * b); //Equation to transform image to grayscale
        pixels[i] = SDL_MapRGBA(format, NULL, grey, grey, grey, a);
    }

    SDL_UnlockSurface(surface);
}

//Check if all pixels satisfy R == G == B
bool is_grey_scale(SDL_Surface* surface) {
    if (!surface) return false;

    SDL_LockSurface(surface);

    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);

    Uint8 r, g, b, a;
    Uint32* pixels = (Uint32*)surface->pixels;
    const int pixel_count = surface->w * surface->h;

    for (int i = 0; i < pixel_count; ++i) {
        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
        if (!(r == g && g == b)) {
            SDL_UnlockSurface(surface);
            return false;
        }
    }

    SDL_UnlockSurface(surface);
    return true;
}
