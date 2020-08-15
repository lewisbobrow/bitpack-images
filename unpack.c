/*
 *  HW4: Machine Arithmetic
 *  unpack.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include "40image.h"
#include "bitpack.h"
#include "arith40.h"

/* storeWord()
 * Purpose: apply function used to map through an empty array equal to
 *          half the dimensions of the original image. In each
 *          cell, get 32 bits of info from the input file and store it.
 * Input: int values representing the position, the array being mapped,
 *        a void * that points to the element at the position i, j in
 *        array2, and a void * to a closure object that can contain anything.
 * Output: n/a
 */
void storeWord(int i, int j, A2 array2, void *elem, void *cl)
{
    (void) elem;
    uint64_t word = 0;

    /* getting 4 chars (32 bits) of data from the input file */
    unsigned byte1 = fgetc((FILE *)cl);
    unsigned byte2 = fgetc((FILE *)cl);
    unsigned byte3 = fgetc((FILE *)cl);
    unsigned byte4 = fgetc((FILE *)cl);

    /* packing these chars in a word in reverse order
    (first character read goes into last 8 bits of word) */
    word = Bitpack_newu(word, 8, 0, byte4);
    word = Bitpack_newu(word, 8, 8, byte3);
    word = Bitpack_newu(word, 8, 16, byte2);
    word = Bitpack_newu(word, 8, 24, byte1);

    assert(word);
    *(uint64_t *)UArray2_at(array2, i, j) = word;
}

/* unpackWord()
 * Purpose: retrieves the values of each component of the info2x2qd struct
 *          (a, b, c, d, aPb, aPr) by accessing the appropriate fields
 *          (given by the lsbInfo format argument) in the word. Then
 *          gets the corresponding quantized value of each of these numbers.
 * Input: uint64_t core-word, lsbInfo struct containing information about
          the lsb and width of each value stored in the word.
 * Output: info2x2dq struct
 */
info2x2dq unpackWord(uint64_t word, lsbInfo format)
{
    uint64_t word64 = word;
    info2x2dq info = (info2x2dq)malloc(sizeof(*info));

    /* unpacking each value based on the lsb and width values
    set in format parameter */
    info->aPb = Arith40_chroma_of_index(Bitpack_getu
                        (word64, format->aPb_w, format->aPb_lsb));
    info->aPr = Arith40_chroma_of_index(Bitpack_getu
                        (word64, format->aPr_w, format->aPr_lsb));

    /* dividing each value by the max range to get a new value within
    the appropriate bounds of quantization */
    info->a = (float)Bitpack_getu(word64, format->a_w, format->a_lsb) / 511;
    info->b = (float)Bitpack_gets(word64, format->b_w, format->b_lsb) / 50;
    info->c = (float)Bitpack_gets(word64, format->c_w, format->c_lsb) / 50;
    info->d = (float)Bitpack_gets(word64, format->d_w, format->d_lsb) / 50;

    return info;
}
