#include "helper.h"
#include <stdio.h>
#include <math.h>

#include "helper.h"

static int read_two_bytes(FILE *fp) {
    int hi = fgetc(fp);
    int lo = fgetc(fp);
    return (hi << 8) | lo;
}

static void write_two_bytes(FILE *fp, int val) {
    fputc((val >> 8) & 0xFF, fp);
    fputc(val & 0xFF, fp);
}

void create(struct rgb **im, size_t h, size_t w) {
    *im = malloc(sizeof(struct rgb));
    (*im)->h = h;
    (*im)->w = w;
    (*im)->r = calloc(3 * h * w, sizeof(uint8_t));
}

void destroy(struct rgb *im) {
    if (im) {
        free(im->r);
        free(im);
    }
}

void read(struct rgb **im, const char *f) {
    FILE *fp = fopen(f, "rb");
    if (!fp) exit(1);
    int h = read_two_bytes(fp);
    int w = read_two_bytes(fp);
    create(im, h, w);
    fread((*im)->r, sizeof(uint8_t), 3 * h * w, fp);
    fclose(fp);
}

void write(struct rgb *im, const char *f) {
    FILE *fp = fopen(f, "wb");
    if (!fp) exit(1);
    write_two_bytes(fp, im->h);
    write_two_bytes(fp, im->w);
    fwrite(im->r, sizeof(uint8_t), 3 * im->h * im->w, fp);
    fclose(fp);
}

uint8_t get(struct rgb *im, int y, int x, int col) {
    int idx = 3 * (y * im->w + x) + col;
    return im->r[idx];
}

void set(struct rgb *im, int y, int x, int r, int g, int b) {
    int idx = 3 * (y * im->w + x);
    im->r[idx] = r;
    im->r[idx + 1] = g;
    im->r[idx + 2] = b;
}

void print(struct rgb *im) {
    for (int i = 0; i < im->h; i++) {
        for (int j = 0; j < im->w; j++) {
            printf("%d\t", get(im, i, j, 0));
        }
        printf("\n");
    }
}
