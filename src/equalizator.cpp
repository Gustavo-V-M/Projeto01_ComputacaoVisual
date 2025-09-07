#include <equalizator.h>

Equalizator::Equalizator(SDL_Surface* surface, Histogram* hist) {
    original_surface = surface;
    original_histogram = hist;
    equalized_surface = equalize();
    equalized_histogram = new Histogram(equalized_surface);
}

Equalizator::~Equalizator() {}

SDL_Surface* Equalizator::get_equalized_surface() {
    if (!equalized_surface) {
        equalized_surface = equalize();
    }
    return equalized_surface;
}

SDL_Surface* Equalizator::get_original_surface() {
    return original_surface;
}

Histogram* Equalizator::get_original_histogram() {
    return original_histogram;
}

Histogram* Equalizator::get_equalized_histogram() {
    if (!(equalized_histogram && equalized_surface)) {
        equalized_histogram = new Histogram(equalized_surface);
    }
    return equalized_histogram;
}

SDL_Surface* Equalizator::equalize() {
    if (!original_surface || !original_histogram) {
        SDL_Log("Equalizator: original_surface or original_histogram is null.");
        return NULL;
    }

    int pixel_count = original_surface->w * original_surface->h;
    SDL_Log("Equalizator: Equalizing surface of size %dx%d (%d pixels)...", original_surface->w, original_surface->h, pixel_count);

    double new_values[256];
    SDL_Log("Equalizator: Calculating new histogram values...");
    for (size_t i = 0; i < 256; i++) {
        double sum = 0.0;
        for (size_t j = 0; j <= i; j++) {
            sum += original_histogram->get_histogram()[j]; 
            // SDL_Log("SUM: i=%zu, j=%zu, hist[j]=%f, sum=%f", i, j, original_histogram->get_histogram()[j], sum);
        }
        new_values[i] = (255 / (original_surface->w * original_surface->h)) * sum;
        // SDL_Log("Equalizator: new_values[%zu] = %d", i, new_values[i]);
    }

    Uint8 pixels[pixel_count];
    Uint32 *original_pixels = (Uint32 *)original_surface->pixels;
    SDL_Log("Equalizator: Mapping pixels to equalized values...");
    for (size_t i = 0; i < pixel_count; i++) {
        Uint8 r, g, b, a;
        const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(original_surface->format);
        SDL_GetRGBA(original_pixels[i], format, NULL, &r, &g, &b, &a);
        pixels[i] = new_values[r];
    }

    SDL_Log("Equalizator: Creating equalized surface...");
    equalized_surface = SDL_CreateSurfaceFrom(original_surface->w, original_surface->h, original_surface->format, pixels, original_surface->pitch);

    if (!equalized_surface) {
        SDL_Log("Equalizator: Failed to create equalized surface.");
    } else {
        SDL_Log("Equalizator: Equalized surface created successfully.");
    }

    return equalized_surface;

}

