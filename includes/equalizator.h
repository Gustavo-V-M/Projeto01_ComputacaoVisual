/* Group Members:
 * Enzo Guarnieri                               RA 10410074
 * Júlia Campolim Oste                          RA 10408802
 * Gustavo Villela Mitraud                      RA 10400866
 * Matheus Marçal Ramos de Oliveira             RA 10409001
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 */
#ifndef EQUALIZATOR_H
#define EQUALIZATOR_H
#include <SDL3/SDL.h>
#include <histogram.h>

class Equalizator {
    public:
        Equalizator(SDL_Surface* surface, Histogram* histogram);
        ~Equalizator();

        SDL_Surface* get_equalized_surface();
        SDL_Surface* get_original_surface();
        Histogram* get_original_histogram();
        Histogram* get_equalized_histogram();
    private:
        SDL_Surface* original_surface;
        SDL_Surface* equalized_surface;
        Histogram* original_histogram;
        Histogram* equalized_histogram;
        SDL_Surface* equalize();
};

#endif // EQUALIZATOR_H
