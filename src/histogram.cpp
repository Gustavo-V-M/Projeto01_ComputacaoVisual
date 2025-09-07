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
bool Histogram::is_high_contrast() { 

    double threshold = 40.0;
    double standard_deviation = 0.0;
    double mean = 0.0;

    double sum = 0.0;
    for (int i = 0; i < 256; i++) {
        sum += histogram[i];
    }
    mean = sum / pixel_count;

    for (int i = 0; i < 256; i++) {
        standard_deviation += pow(histogram[i] - mean, 2);
    }

    SDL_Log("Standard Deviation: %f", sqrt(standard_deviation / pixel_count));

    return (sqrt(standard_deviation / pixel_count) > threshold);

 } // TODO: implementar
bool Histogram::is_dark() { 
    int light_pixels = 0;
    int dark_pixels = 0;
    int threshold = 128; 
    for (int i = 0; i < 256; i++) {
        if (i < threshold) {
            dark_pixels += histogram[i];
        } else {
            light_pixels += histogram[i];
        }
    }

    SDL_Log("Dark pixels: %d, Light pixels: %d", dark_pixels, light_pixels);

    return dark_pixels >= light_pixels;
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


