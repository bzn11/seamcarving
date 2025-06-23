#ifndef HELPER_H
#define HELPER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct rgb {
    int h;
    int w;
    uint8_t *r; 
}
void create(struct rgb **im, size_t h, size_t w);
void destroy(struct rgb *im);


void read(struct rgb **im, const char *f);
void write(struct rgb *im, const char *f);

uint8_t get(struct rgb *im, int y, int x, int col);
void set(struct rgb *im, int y, int x, int r, int g, int b);

void print(struct rgb *im);

#endif
