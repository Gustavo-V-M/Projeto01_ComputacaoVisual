/* Group Members:
 * Enzo Guarnieri                               RA 10410074
 * Júlia Campolim Oste                          RA 10408802
 * Gustavo Villela Mitraud                      RA 10400866
 * Matheus Marçal Ramos de Oliveira             RA 10409001
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 */
#include <histogram.h>
#include <SDL3/SDL.h>
#include <cmath>


Histogram::Histogram(SDL_Surface *surface) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(surface->format);
    const size_t pixelCount = surface->w * surface->h;
    pixel_count = pixelCount;

    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint8 r, g, b, a;

    for (size_t i = 0; i < pixelCount; i++) {
        SDL_GetRGBA(pixels[i], format, NULL, &r, &g, &b, &a);
        histogram[r]++;
    }
}

Histogram::~Histogram() {};
double *Histogram::get_histogram() { return histogram; }

 float Histogram::mean_intensity() {
    double total_intensity = 0.0;
    int pixel_count = 0;

    for (int i = 0; i < 256; i++) {
        total_intensity += (histogram[i] * i);
        pixel_count += histogram[i];
    }

    double mean = total_intensity / pixel_count;
    SDL_Log("Mean Intensity: %f", mean);

    return mean;
}

double Histogram::standard_deviation() { 
    double total_intensity = 0.0;
    int pixel_count = 0;

    for (int i = 0; i < 256; i++) {
        total_intensity += histogram[i] * i;
        pixel_count += histogram[i];
    }

    if (pixel_count == 0)
        return 0.0;

    double mean = total_intensity / pixel_count;

    double variance = 0.0;
    for (int i = 0; i < 256; i++) {
        double diff = i - mean;
        variance += histogram[i] * diff * diff;
    }

    variance /= pixel_count;

    double standard_deviation = sqrt(variance);
    SDL_Log("Standard Deviation: %f", standard_deviation);

    return standard_deviation;
} 

 SDL_Surface *Histogram::render_histogram(SDL_Renderer* renderer, int width, int height) {
    if (!renderer || width <= 0 || height <= 0) return nullptr;

    SDL_Surface *surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    const SDL_PixelFormatDetails *format_details = SDL_GetPixelFormatDetails(surface->format);
    if (!surface) {
        SDL_Log("SDL_CreateRGBSurface failed: %s", SDL_GetError());
        return nullptr;
    }

    // Find the maximum value in the histogram for normalization
    int max_value = 0;
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > max_value) {
            max_value = histogram[i];
        }
    }

    // Draw the histogram
    for (int x = 0; x < width; x++) {
        int bin = (x * 256) / width;
        int bin_height = (histogram[bin] * height) / max_value;

        for (int y = height - 1; y >= height - bin_height; y--) {
            Uint32* pixels = (Uint32*)surface->pixels;
            pixels[y * width + x] = SDL_MapRGBA(format_details, NULL, 255, 255, 255, 255); // White color
        }
    }

    return surface;
 }


