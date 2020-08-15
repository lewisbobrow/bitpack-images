/*
 *  HW4: Machine Arithmetic
 *  dequantize.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include "40image.h"
#include "bitpack.h"

/* wordToRGB()
 * Purpose: Convert/move a given 32-bit code word contained with a UArray2_T
            object to a 2x2 neighborhood of RGB pixels, contained within
            another UArray2_T object, whose height/width are both 2* the
            height/width of the word UArray2_T. It does this by calling
            various functions to unpack the 32-bit code word into an info2x2dq
            struct object, converting/moving the info in that object into Y,
            Pb, and Pr values contained within a cvsPix struct object, and
            converting/moving those values into RGB values in a regular RGB
            pixel.
 * Input: An (i, j) position with a given UArray2_T, the UArray2_T object, a
          given element within that UArray2_T RGB pixel object, and a closure
          variable to represent the UArray2_T object used for storing RGB
          pixels.
 * Output: N/A
 */
void wordToRGB(int i, int j, A2 array2, void *elem, void *cl)
{
    (void) array2;
    lsbInfo format = setLSB(23, 18, 13, 8, 4, 0);
    info2x2dq info = unpackWord(*(uint64_t *)(UArray2_at(
                        (UArray2_T)cl, i / 2, j / 2)), format);

    cvsPix newPix = info2x2dqToCvs(info, i, j);
    Pnm_rgb color = cvsToRGBPix(newPix->Y, newPix->Pb, newPix->Pr, 65535);
    ((Pnm_rgb)elem)->red = color->red;
    ((Pnm_rgb)elem)->green = color->green;
    ((Pnm_rgb)elem)->blue = color->blue;

    free(color);
    free(newPix);
    free(info);
    free(format);
}

/* info2x2dqToCvs()
 * Purpose: Converts info2x2 values into CVS Pixel values by checking if the
            current position in the mapping is a top-left, top-right, bot-left
            or bot-right pixel, then applying the transformation.
 * Input:   info2x2dq struct containing the dequantized values, two int values
 *          corresponding to the current position of the big UArray2_T
 * Output:  cvsPix struct initialized with appropriate values for given index
 */
cvsPix info2x2dqToCvs(info2x2dq info, int i, int j)
{
    cvsPix newPixel = (cvsPix)malloc(sizeof(*newPixel));

    if (i % 2 == 0) {
        if (j % 2 == 0) { //top left of 2x2
            newPixel->Y = (info->a) - (info->b) - (info->c) + (info->d); // Y1
        } else { //bottom left of 2x2
            newPixel->Y = (info->a) + (info->b) - (info->c) - (info->d); // Y3
        }
    } else {
        if (j % 2 == 0){ //top right of 2x2
            newPixel->Y = (info->a) - (info->b) + (info->c) - (info->d); // Y2
        } else{ //bottom right of 2x2
            newPixel->Y = (info->a) + (info->b) + (info->c) + (info->d); //Y4
        }
    }

    newPixel->Pb = info->aPb;
    newPixel->Pr = info->aPr;

    return newPixel;
}

/* cvsToRGBPix()
 * Purpose: Calculates the appropriate rgb values based on the given
 *          component video values.
 * Input: float value, float value, float value, int value
 * Output: Pnm_rgb struct
 */
Pnm_rgb cvsToRGBPix(float Y, float Pb, float Pr, int denom)
{
    Pnm_rgb newPixel = (Pnm_rgb)malloc(sizeof(*newPixel));

    /* if rgb value is greater than the deomoninator due to a precision error,
       the value gets set to the denominator */
    int redVal = round((1.0 * Y + 0.0 * Pb + 1.402 * Pr) * denom);
    if (redVal > denom) {
        redVal = denom;
    }

    int greenVal = round((1.0 * Y - 0.344136 * Pb - 0.714136 * Pr) * denom);
    if (greenVal > denom) {
        greenVal = denom;
    }

    int blueVal = round((1.0 * Y + 1.772 * Pb + 0.0 * Pr) * denom);
    if (blueVal > denom) {
        blueVal = denom;
    }

    /* if the resulting rgb values are negative, set them to 0 */
    if (redVal < 0) {
        redVal = 0;
    }

    if (greenVal < 0) {
        greenVal = 0;
    }

    if (blueVal < 0) {
        blueVal = 0;
    }

    newPixel->red = redVal;
    newPixel->green = greenVal;
    newPixel->blue = blueVal;
    return newPixel;
}
