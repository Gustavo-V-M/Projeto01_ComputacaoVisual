/* Group Members:
 * Enzo Guarnieri                               RA 10410074
 * Júlia Campolim Oste                          RA 10408802
 * Gustavo Villela Mitraud                      RA 10400866
 * Matheus Marçal Ramos de Oliveira             RA 10409001
 * Sabrina Midori Futami Teixeira de Carvalho   RA 10410220
 */
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SDL3/SDL.h>

class Histogram {
    public:
        Histogram(SDL_Surface *surface);
        ~Histogram();

        SDL_Surface* render_histogram(int width, int height);
        float mean_intensity();
        double standard_deviation();
        double *get_histogram();
    private:
        double histogram[256];
        int pixel_count;
};

#endif
