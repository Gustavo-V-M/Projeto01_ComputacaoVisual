/* Group Members:
 * Enzo Guarnieri                               RA 10410074
 * Júlia Campolim Oste                          RA 10408802
 * Gustavo Villela Mitraud                      RA 10400866
 * Matheus Marçal Ramos de Oliveira             RA 10409001
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 */
#include <equalizator.h>

Equalizator::Equalizator(SDL_Surface *surface, Histogram *hist)
{
    original_surface = surface;
    original_histogram = hist;
    equalized_histogram = NULL;
    equalized_surface = NULL;
}

Equalizator::~Equalizator() {}

SDL_Surface *Equalizator::get_equalized_surface()
{
    if (!equalized_surface)
    {
        equalized_surface = equalize();
    }
    return equalized_surface;
}

SDL_Surface *Equalizator::get_original_surface()
{
    return original_surface;
}

Histogram *Equalizator::get_original_histogram()
{
    return original_histogram;
}

Histogram *Equalizator::get_equalized_histogram()
{
    if (!(equalized_histogram && equalized_surface))
    {
        equalized_histogram = new Histogram(equalized_surface);
    }
    return equalized_histogram;
}

SDL_Surface *Equalizator::equalize()
{
    if (!original_surface || !original_histogram)
    {
        SDL_Log("Equalizator: original_surface or original_histogram is null.");
        return NULL;
    }
    Uint8 new_values_lookuptable[256];

    for (size_t i = 0; i < 256; i++)
    {
        int sum = 0;
        for (size_t j = 0; j < i; j++)
        {
            sum += original_histogram->get_histogram()[j];
        }
        double unrounded = (255.0 / (original_surface->w * original_surface->h)) * sum;
        new_values_lookuptable[i] = SDL_round(unrounded);
    }

    SDL_Surface *new_surface = SDL_CreateSurface(original_surface->w, original_surface->h, original_surface->format);
    if (!new_surface)
    {
        SDL_Log("Equalizator: SDL_CreateSurface failed: %s", SDL_GetError());
        return NULL;
    }
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(new_surface->format);
    const size_t pixelCount = new_surface->w * new_surface->h;

    Uint32 *new_pixels = (Uint32 *)new_surface->pixels;
    Uint32 *original_pixels = (Uint32 *)original_surface->pixels;
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;
    Uint8 a = 0;

    for (size_t i = 0; i < pixelCount; ++i)
    {
        SDL_GetRGBA(original_pixels[i], format, NULL, &r, &g, &b, &a);
        Uint8 y = new_values_lookuptable[r];

        new_pixels[i] = SDL_MapRGBA(format, NULL, y, y, y, a);
    }

    return new_surface;
}
