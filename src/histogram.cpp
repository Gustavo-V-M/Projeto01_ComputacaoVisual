#include <histogram.h>
#include <SDL3/SDL.h>


Histogram::Histogram(SDL_Surface* surface) {
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    // TODO: finalizar implementação do construtor
}

Histogram::~Histogram() {};

bool Histogram::is_high_contrast() { return false; } // TODO: implementar
bool Histogram::is_dark() { return false; } // TODO: implementar
