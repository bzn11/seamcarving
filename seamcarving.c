#include "seamcarving.h"
#include "helper.h"
#include <stdio.h>
#include <math.h>

void calc_energy(struct rgb_img *im, struct rgb_img **grad) {
    int height = im->height;
    int width = im->width;
    create_img(grad, height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int left = j - 1;
            int right = j + 1;
            int up = i - 1;
            int down = i + 1;

            if (left < 0) left = width - 1;
            if (right >= width) right = 0;
            if (up < 0) up = height - 1;
            if (down >= height) down = 0;

            int rx = get_pixel(im, i, left, 0) - get_pixel(im, i, right, 0);
            int gx = get_pixel(im, i, left, 1) - get_pixel(im, i, right, 1);
            int bx = get_pixel(im, i, left, 2) - get_pixel(im, i, right, 2);

            int ry = get_pixel(im, up, j, 0) - get_pixel(im, down, j, 0);
            int gy = get_pixel(im, up, j, 1) - get_pixel(im, down, j, 1);
            int by = get_pixel(im, up, j, 2) - get_pixel(im, down, j, 2);

            int dx = rx * rx + gx * gx + bx * bx;
            int dy = ry * ry + gy * gy + by * by;

            int energy = sqrt(dx + dy);
            uint8_t e = energy / 10;
            set_pixel(*grad, i, j, e, e, e);
        }
    }
}


void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    int height = grad->height;
    int width = grad->width;
    *best_arr = (double *)malloc(sizeof(double) * height * width);

    for (int j = 0; j < width; j++) {
        int pixel_val = get_pixel(grad, 0, j, 0); 
        (*best_arr)[j] = (double)pixel_val;
    }

    for (int i = 1; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double min_val;

            double left = INFINITY;              
            double up = (*best_arr)[(i - 1) * width + j];
            double right = INFINITY;

            if (j > 0) {
                left = (*best_arr)[(i - 1) * width + (j - 1)];
            }
            if (j < width - 1) {
                right = (*best_arr)[(i - 1) * width + (j + 1)];
            }

            if (left < up && left < right) {
                min_val = left;
            } else if (up < right) {
                min_val = up;
            } else {
                min_val = right;
            }

            int current_energy = get_pixel(grad, i, j, 0);
            (*best_arr)[i * width + j] = (double)current_energy + min_val;
        }
    }
}


void recover_path(double *best_arr, int height, int width, int **path) {
    *path = (int *)malloc(sizeof(int) * height);

    int min_index = 0;
    for (int j = 1; j < width; j++) {
        if (best_arr[(height - 1) * width + j] < best_arr[(height - 1) * width + min_index]) {
            min_index = j;
        }
    }

    (*path)[height - 1] = min_index;

    for (int i = height - 2; i >= 0; i--) {
        int prev_index = (*path)[i + 1];

        int left = prev_index - 1;
        int right = prev_index + 1;
        int middle = prev_index;

        double left_val = INFINITY;
        double middle_val = best_arr[i * width + middle];
        double right_val = INFINITY;

        if (left >= 0) {
            left_val = best_arr[i * width + left];
        }
        if (right < width) {
            right_val = best_arr[i * width + right];
        }

        if (left_val < middle_val && left_val < right_val) {
            (*path)[i] = left;
        } else if (middle_val < right_val) {
            (*path)[i] = middle;
        } else {
            (*path)[i] = right;
        }
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path) {
    int height = src->height;
    int width = src->width;

    create_img(dest, height, width - 1);

    for (int i = 0; i < height; i++) {
        int seam_col = path[i];
        int new_col = 0;

        for (int j = 0; j < width; j++) {
            if (j != seam_col) {
                uint8_t r = get_pixel(src, i, j, 0);
                uint8_t g = get_pixel(src, i, j, 1);
                uint8_t b = get_pixel(src, i, j, 2);
                set_pixel(*dest, i, new_col, r, g, b);
                new_col++;
            }
        }
    }
}
