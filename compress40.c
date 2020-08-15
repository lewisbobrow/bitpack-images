/*
 *  HW4: Machine Arithmetic
 *  compress40.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "assert.h"
#include "math.h"
#include "pnm.h"
#include "compress40.h"
#include "a2plain.h"
#include "uarray2.h"
#include "a2methods.h"
#include "40image.h"
#include "bitpack.h"

/* compress40()
 * Purpose: Compresses a given Pnm_ppm file into a series of 32-bit code
            words, by reading in the image file from stdin or the command
            line, making sure the image's width and height are both even,
            converting/moving the RGB values of the Pnm_ppm's pixels into
            component-video-space values contained in a UArray2_T object
            of cvsPix struct objects, converting the CVS values into
            a/b/c/d/aPb/aPr values contained in a UArray2_T object of info2x2
            struct objects (representing a 2x2 pixel neighborhood within the
            original image), printing out the code words and appropriate
            info, and freeing any used memory.
 * Input: A ppm file to be compressed.
 * Output: N/A
 */
void compress40(FILE *input)
{
    Pnm_ppm image = Pnm_ppmread(input, uarray2_methods_plain);
    image = trimOddEdges(image);

    /* initializing empty UArray2_T to store converted cvs pixels */
    cvsPix size;
    UArray2_T cvsPixArray = UArray2_new(image->width, image->height,
                                        sizeof(*size));
    uarray2_methods_plain->map_default(cvsPixArray, convertToCvsPix, image);

    /* initializing empty UArray2_T of half dimensions to store info2x2
       structs */
    info2x2 temp;
    UArray2_T infoArray = UArray2_new(image->width / 2, image->height / 2,
                                      sizeof(*temp));

    /* printing header then mapping over compressed UArray2 to print each
       code word */
    printf("COMP40 Compressed image format 2\n%u %u\n", image->width,
                                                        image->height);
    uarray2_methods_plain->map_default(infoArray, compressCvsPix, cvsPixArray);
    UArray2_free(&infoArray);
    UArray2_free(&cvsPixArray);
    Pnm_ppmfree(&image);
    fclose(input);
    exit(EXIT_SUCCESS);
}

/* trimOddEdges()
 * Purpose: Trims the edges of a given Pnm_ppm image to make both its width
            and its height even, if its width and/or its height are odd.
 * Input: An existing and valid Pnm_ppm image, possibly with odd height/width
 * Output: A new Pnm_ppm image with an even height/width.
 */
Pnm_ppm trimOddEdges(Pnm_ppm image)
{
    /* checks if height is odd */
    if (image->height % 2 != 0) {
	    image->height = (image->height) - 1;
    }

    /* checks if width is odd */
    if (image->width % 2 != 0) {
	    image->width = (image->width) - 1;
    }
    return image;
}

/* decompress40()
 * Purpose: reads 32-bit code words from an input file, stores them in a
 *          UArray2_T object, then maps through a UArray2_T object with
 *          double these dimensions, dequantizes the values stored in each
 *          word, then converts these values to rgb. Prints out the resulting
 *          UArray2_T using Pnm_ppmwrite.
 * Input: FILE *object
 * Output: n/a
 */
void decompress40(FILE *input)  /* read compressed image, write PPM */
{
    /* ensure that the header of the input file is appropriate and advance
       past it to begin reading words */
    unsigned w, h;      // width, height
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u%u", &w, &h);
    assert (read == 2);
    int c = getc(input);
    assert(c == '\n');

    /* initializes Pnm_ppm object with an empty UArray2_T that is to be
       filled with Pnm_rgb objects */
    Pnm_rgb temp;
    Pnm_ppm pixmap = (Pnm_ppm)malloc(sizeof(*pixmap));
    pixmap->width = w;
    pixmap->height = h;
    pixmap->denominator = 65535;
    pixmap->pixels = UArray2_new(w, h, sizeof(*temp));
    pixmap->methods = uarray2_methods_plain;

    /* initializes empty UArray2_T that will store all code words in their
       appropriate positions */
    UArray2_T words = UArray2_new(w / 2, h / 2, sizeof(uint64_t));

    /* reads in 32 bits at a time from the input file and stores it in
       the words array */
    uarray2_methods_plain->map_default(words, storeWord, input);


    /* gets appropriate rgb pixel from the words array, then stores rgb
       value in pixels array */
    uarray2_methods_plain->map_default(pixmap->pixels, wordToRGB, words);

    Pnm_ppmwrite(stdout, pixmap);

    UArray2_free(&words);
    fclose(input);
    Pnm_ppmfree(&pixmap);

    exit(EXIT_SUCCESS);
}
