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

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SDL3/SDL.h>

class Histogram {
    public:
        Histogram(SDL_Surface *surface);
        ~Histogram();

        bool is_high_contrast();
        bool is_dark();
        SDL_Surface* render_histogram(SDL_Renderer* renderer, int width, int height);
        double *get_histogram();
    private:
        double histogram[256];
        int pixel_count;
};

#endif