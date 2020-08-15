#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "compress40.h"
#include "math.h"
#include "pnm.h"
#include "a2plain.h"
#include "uarray2.h"
#include "uarray.h"
#include "a2methods.h"
#include "40image.h"

int main(void)
{
    Pnm_rgb rgbPixel = (Pnm_rgb)malloc(sizeof(*rgbPixel));
    rgbPixel->red = 200;
    rgbPixel->green = 100;
    rgbPixel->blue = 0;

    float r = (float)rgbPixel->red / 255;
    float g = (float)rgbPixel->green / 255;
    float b = (float)rgbPixel->blue / 255;

    cvsPix cvsPixel = rgbToCvsPix(r, g, b);
    // expected values:
    // Y = 0.46470588235 (from 0 to 1)
    // Pb = -0.26224941176 (from -0.5 to +0.5)
    // Pr = 0.22796549019 (from -0.5 to +0.5)

    printf("**COMPRESS TESTS**")
    printf("**Converting rgb to cvs**\n");
    printf("rgbPixel r: %f, g: %f, b: %f\n", r, g, b);
    printf("cvsPixel Y: %f, Pb: %f, Pr: %f\n\n",
        cvsPixel->Y, cvsPixel->Pb, cvsPixel->Pr);



    Pnm_rgb rgbPix2 = cvsToRGBPix(cvsPixel->Y, cvsPixel->Pb,
                                      cvsPixel->Pr, 255);
    printf("**Converting cvs to rgb**\n");
    printf("cvsPixel Y: %f, Pb: %f, Pr: %f\n",
        cvsPixel->Y, cvsPixel->Pb, cvsPixel->Pr);
    printf("rgbPixel r: %u, g: %u, b: %u\n\n",
        rgbPix2->red, rgbPix2->green, rgbPix2->blue);



    cvsPix cvsPixel1 = (cvsPix)malloc(sizeof(*cvsPixel1));
    cvsPixel1->Y = 0.8;
    cvsPixel1->Pb = -0.4;
    cvsPixel1->Pr = 0.4;
    cvsPix cvsPixel2 = (cvsPix)malloc(sizeof(*cvsPixel2));
    cvsPixel2->Y = 0.1;
    cvsPixel2->Pb = 0.3;
    cvsPixel2->Pr = 0.5;
    cvsPix cvsPixel3 = (cvsPix)malloc(sizeof(*cvsPixel3));
    cvsPixel3->Y = 0.9;
    cvsPixel3->Pb = -0.5;
    cvsPixel3->Pr = 0.2;
    cvsPix cvsPixel4 = (cvsPix)malloc(sizeof(*cvsPixel4));
    cvsPixel4->Y = 0.5;
    cvsPixel4->Pb = -0.1;
    cvsPixel4->Pr = 0;
    float aPb = (cvsPixel1->Pb + cvsPixel2->Pb + cvsPixel3->Pb + cvsPixel4->Pb)/4;
    float aPr = (cvsPixel1->Pr + cvsPixel2->Pr + cvsPixel3->Pr + cvsPixel4->Pr)/4;
    info2x2 pixInfo = cvsToInfo2x2(cvsPixel1->Y, cvsPixel2->Y,
                                       cvsPixel3->Y, cvsPixel4->Y, aPb, aPr);
    // expected values:
    // a = (0.5 +0.9 +0.1 +0.8) / 4.0 = 0.575
    // b = (0.5 + 0.9 − 0.1 − 0.8) / 4.0 = 0.125
    // c = (0.5 − 0.9 + 0.1 − 0.8) / 4.0 = -0.275
    // d = (0.5 −0.9 −0.1 +0.8) / 4.0 = 0.075

    printf("**Converting cvs to info2x2**\n");
    printf("cvsPixel Y1: %f, Y2: %f, Y3: %f, Y4: %f, aPb: %f, aPr: %f\n",
        cvsPixel1->Y, cvsPixel2->Y, cvsPixel3->Y, cvsPixel4->Y, aPb, aPr);
    printf("pixInfo a: %f, b: %f, c: %f, d: %f, aPb: %f, aPr: %f\n\n",
        pixInfo->a, pixInfo->b, pixInfo->c, pixInfo->d,
        pixInfo->aPb, pixInfo->aPr);


    cvsPix pixels[4] = (cvsPix)malloc(sizeof(*cvsPix * 4));
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            pixels[(i * 2) + j]= info2x2ToCvs(pixInfo, int i, int j);
        }
    }
    printf("**Converting info2x2 to cvs**\n");
    printf("pixInfo a: %f, b: %f, c: %f, d: %f, aPb: %f, aPr: %f\n\n",
        pixInfo->a, pixInfo->b, pixInfo->c, pixInfo->d,
        pixInfo->aPb, pixInfo->aPr);
    printf("cvsPixel Y1: %f, Y2: %f, Y3: %f, Y4: %f, aPb: %f, aPr: %f\n",
        cvsPixel1->Y, cvsPixel2->Y, cvsPixel3->Y, cvsPixel4->Y, aPb, aPr);

    return 0;
}
