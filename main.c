#include <stdio.h>
#include <stdlib.h>
#include "seamcarving.h"
#include "helper.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_image.bin> <num_seams_to_remove>\n", argv[0]);
        return 1;
    }

    struct rgb *image = NULL;
    read(&image, argv[1]);

    int seams = atoi(argv[2]);
    struct rgb *cur_img = image;

    for (int i = 0; i < seams; i++) {
        struct rgb *energy_img = NULL;
        calc_energy(cur_img, &energy_img);

        double *cost_array = NULL;
        dynamic_seam(energy_img, &cost_array);

        int *path = NULL;
        recover_path(cost_array, energy_img->h, energy_img->w, &path);

        struct rgb *new_img = NULL;
        remove_seam(cur_img, &new_img, path);

        write(new_img, "output.bin");

        destroy(cur_img);
        destroy(energy_img);
        free(cost_array);
        free(path);

        cur_img = new_img;

        printf("Removed seam %d/%d\n", i+1, seams);
    }

    destroy(cur_img);
    return 0;
}
