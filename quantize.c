/*
 *  HW4: Machine Arithmetic
 *  quantize.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include "40image.h"
#include "bitpack.h"

/* convertToCvsPix()
 * Purpose: Apply function that converts RGB Pixels to CVS Pixels by getting
 *          each rgb value on a scale from 0 to 1, then applying the
 *          corresponding transformation to get Y, Pb and Pr.
 * Input: int value, int value, A2 object, void *object, void *object
 * Output: n/a
 */
void convertToCvsPix(int i, int j, A2 array2, void *elem, void *cl)
{
    (void) array2;
    Pnm_rgb oldPixel = (Pnm_rgb)UArray2_at(((Pnm_ppm)cl)->pixels, i, j);
    assert(oldPixel);

    /* dividing by denominator to get value between 0 and 1 */
    float r = (float)oldPixel->red / ((Pnm_ppm)cl)->denominator;
    float g = (float)oldPixel->green / ((Pnm_ppm)cl)->denominator;
    float b = (float)oldPixel->blue / ((Pnm_ppm)cl)->denominator;

    /* applying transformation functions */
    ((cvsPix)elem)->Y = 0.299 * r + 0.587 * g + 0.114 * b;
    ((cvsPix)elem)->Pb = -0.168736 * r - 0.331264 * g + 0.5 * b;
    ((cvsPix)elem)->Pr = 0.5 * r - 0.418688 * g - 0.081312 * b;
}

/* compressCvsPix()
 * Purpose: Apply function that converts CVS Pixel into info2x2 struct
 *          by mapping over the half-dimension array and getting the 2x2
 *          pixel block corresponding with each cell. Uses the values from
 *          this pixel block to create an info2x2 struct containing the
 *          a, b, c, d, aPb and aPr values of each struct, quantizing these
 *          values, then storing them in a word. Prints this word to stdout
 *          by printing the most significant byte first.
 *
 * Input: int values representing the position, the array being mapped,
 *        a void * that points to the element at the position i, j in
 *        array2, and a void * to a closure object that can contain anything.
 * Output: n/a
 */
void compressCvsPix(int i, int j, A2 array2, void *elem, void *cl)
{
    (void) array2;
    (void) elem;
    /* gets the top-left, top-right, bot-left and bot-right positions
       in the image array corresponding to a 2x2 block */
    cvsPix cvsPix1 = (cvsPix)UArray2_at((UArray2_T)cl, 2 * i, 2 * j);
    cvsPix cvsPix2 = (cvsPix)UArray2_at((UArray2_T)cl, 2 * i + 1, 2 * j);
    cvsPix cvsPix3 = (cvsPix)UArray2_at((UArray2_T)cl, 2 * i, 2 * j + 1);
    cvsPix cvsPix4 = (cvsPix)UArray2_at((UArray2_T)cl, 2 * i + 1, 2 * j + 1);
    assert(cvsPix1 && cvsPix2 && cvsPix3 && cvsPix4);

    /* calculates Y1, Y2, Y3, Y4, aPb, and aPr based on these 4 pixels */
    float Y1 = cvsPix1->Y;
    float Y2 = cvsPix2->Y;
    float Y3 = cvsPix3->Y;
    float Y4 = cvsPix4->Y;
    float aPb = (cvsPix1->Pb + cvsPix2->Pb + cvsPix3->Pb + cvsPix4->Pb)/4;
    float aPr = (cvsPix1->Pr + cvsPix2->Pr + cvsPix3->Pr + cvsPix4->Pr)/4;

    info2x2 tempInfo = cvsToInfo2x2(Y1, Y2, Y3, Y4, aPb, aPr);
    lsbInfo format = setLSB(23, 18, 13, 8, 4, 0);   // sets format of lsb
    uint64_t word = packInfo2x2(tempInfo, format);  // bit packs values
    (void) word;

    /* print word so that most significant character comes first */
    putchar((int)Bitpack_gets(word, 8, 24));
    putchar((int)Bitpack_gets(word, 8, 16));
    putchar((int)Bitpack_gets(word, 8, 8));
    putchar((int)Bitpack_gets(word, 8, 0));


    free(tempInfo);
    free(format);
}

/* cvsToInfo2x2()
 * Purpose: Create an info2x2 struct object from the Y values of, and average
            Pb and Pr values amongst, a 2x2 neighborhood of cvsPix struct
            objects. Does so by calculating a, b, c, and d values from the 4
            Y values, then returning the info2x2 struct object created by
            quantizeInfo2x2 (containing a, b, c, d, aPb, and aPr values)
 * Input: Six float values representing the Y values for each cvsPix struct
          object in a 2x2 neighborhood, as well as aPb and aPr values for
          those objects
 * Output: An info2x2 struct with quantized values
 */
info2x2 cvsToInfo2x2(float Y1, float Y2, float Y3, float Y4,
                         float aPb, float aPr)
{
    float a = (Y4 + Y3 + Y2 + Y1) / 4.0;
    float b = (Y4 + Y3 - Y2 - Y1) / 4.0;
    float c = (Y4 - Y3 + Y2 - Y1) / 4.0;
    float d = (Y4 - Y3 - Y2 + Y1) / 4.0;
    return quantizeInfo2x2(a, b, c, d, aPb, aPr);
}
