#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"
#include "math.h"
#include "pnm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "uarray2.h"

void ppmdiff(int argc, char *argv[]);
void computeE(Pnm_ppm image1, Pnm_ppm image2, int h, int w, A2Methods_T methods);

int main(int argc, char *argv[]) {
    ppmdiff(argc, argv);
    return 0;
}

void ppmdiff(int argc, char *argv[]) {
    if (argc != 3) exit(EXIT_FAILURE);
    FILE *fp[2];

    if (strcmp(argv[1], "-") == 0) {
        fp[0] = stdin;
    } else {
        fp[0] = fopen(argv[1], "r");
    }

    if (strcmp(argv[2], "-") == 0) {
        fp[1] = stdin;
    } else {
        fp[1] = fopen(argv[2], "r");
    }

    printf("Reading in\n");
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    Pnm_ppm I1 = Pnm_ppmread(fp[0], uarray2_methods_plain);
    printf("DENOM OF IMAGE 1: %d\n", I1->denominator);
    Pnm_ppm I2 = Pnm_ppmread(fp[1], uarray2_methods_plain);
    printf("Denominator of image2: %d\n", I2->denominator);
    int h_diff = (I1->height) - (I2->height);
    int w_diff = (I1->width) - (I2->width);
    int h = 0;
    int w = 0;
    if ((h_diff > 1) || (h_diff < -1)) {
        fprintf(stderr, "Height difference is too much!\n");
        printf("1.0\n");
    } else if (h_diff >= 0) {
        h = I2->height;
    } else if (h_diff < 0) {
        h = I1->height;
    }

    if ((w_diff > 1) || (w_diff < -1)) {
        fprintf(stderr, "Width difference is too much!\n");
        printf("1.0\n");
    } else if (w_diff >= 0) {
        w = I2->width;
    } else if (w_diff < 0) w = I1->width;

    w_diff = abs(w_diff);
    h_diff = abs(h_diff);

    printf("Computing E\n");
    printf("w_diff: %d\n", w_diff);
    printf("h_diff: %d\n", h_diff);
    printf("w: %d\n", w);
    printf("h: %d\n", h);
    computeE(I1, I2, h, w, methods);
}

void computeE(Pnm_ppm image1, Pnm_ppm image2, int h, int w, A2Methods_T methods) {
    int i, j;
    float e = 0;
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            Pnm_rgb pixel1 = (Pnm_rgb)methods->at(image1->pixels, i, j);
            Pnm_rgb pixel2 = (Pnm_rgb)methods->at(image2->pixels, i, j);
            int redDiff = (pixel1->red / image1->denominator) -
                          (pixel2->red / image2->denominator);
            redDiff = redDiff * redDiff;
            int greenDiff = (pixel1->green / image1->denominator) -
                            (pixel2->green / image2->denominator);
            greenDiff = redDiff * greenDiff;
            int blueDiff = (pixel1->blue / image1->denominator) -
                           (pixel2->blue / image2->denominator);
            blueDiff = blueDiff * blueDiff;
            e += (float)(redDiff + greenDiff + blueDiff);
        }
    }
    e = e / (3 * w * h);
    e = (float)sqrt(e);
    printf("E: %.4f\n", e);
}
