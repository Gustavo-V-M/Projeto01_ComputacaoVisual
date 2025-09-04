#ifndef HISTOGRAM_H
#define HISTOGRAM_H


class Histogram {
    public:
        Histogram(SDL_Surface* surface);
        ~Histogram();

        bool is_high_contrast();
        bool is_dark();
    private:
        int histogram[256];
};

#endif