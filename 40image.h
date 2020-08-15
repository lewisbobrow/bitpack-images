/*
 *  HW4: Machine Arithmetic
 *  40image.h
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "math.h"
#include "pnm.h"
#include "a2plain.h"
#include "uarray2.h"
#include "uarray.h"
#include "a2methods.h"
#include "compress40.h"

typedef A2Methods_UArray2 A2;

// contains float values for the Y, Pb, and Pr values of each RGB pixel in an
// image
typedef struct cvsPix {
    float Y, Pb, Pr;
} *cvsPix;

// contains quantized integer values for a, b, c, d, aPb, and aPr values
typedef struct info2x2 {
    unsigned a, aPb, aPr;
    int b, c, d;
} *info2x2;

// contains dequantized float values for a, b, c, d, aPb, and aPr values
typedef struct info2x2dq {
    float a, b, c, d, aPb, aPr;
} *info2x2dq;

// contains the widths and least significant bits for encoded a, b, c, d, aPb,
// and aPr values in a given 32-bit code word.
typedef struct lsbInfo {
    unsigned a_w, b_w, c_w, d_w, aPb_w, aPr_w;  // widths for listed variables
    unsigned a_lsb, b_lsb, c_lsb, d_lsb, aPb_lsb, aPr_lsb;
} *lsbInfo;


void compress40(FILE *input);
Pnm_ppm trimOddEdges(Pnm_ppm image);
void convertToCvsPix(int i, int j, A2 array2, void *elem, void *cl);
void compressCvsPix(int i, int j, A2 array2, void *elem, void *cl);
info2x2 cvsToInfo2x2(float Y1, float Y2, float Y3, float Y4,
                         float aPb, float aPr);
uint64_t packInfo2x2(info2x2 info, lsbInfo format);
info2x2 quantizeInfo2x2(float a, float b, float c, float d, float aPb,
                        float aPr);
int findQuantized(float num);
lsbInfo setLSB(unsigned a_lsb, unsigned b_lsb, unsigned c_lsb,
               unsigned d_lsb, unsigned aPb_lsb, unsigned aPr_lsb);
void decompress40(FILE *input);
void storeWord(int i, int j, A2 array2, void *elem, void *cl);
void wordToRGB(int i, int j, A2 array2, void *elem, void *cl);
info2x2dq unpackWord(uint64_t word, lsbInfo format);
cvsPix info2x2dqToCvs(info2x2dq info, int i, int j);
Pnm_rgb cvsToRGBPix(float Y, float Pb, float Pr, int denom);
