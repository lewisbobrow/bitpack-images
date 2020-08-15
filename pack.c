/*
 *  HW4: Machine Arithmetic
 *  pack.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include "40image.h"
#include "bitpack.h"
#include "arith40.h"

/* quantizeInfo2x2()
 * Purpose: Quantizes a, b, c, d, aPr, and aPr values, using findQuantized()
 *          and index_of_chroma(), then assigns those quantized values to
 *          corresponding values in a new info2x2 struct object.
 * Input: a, b, c, d, aPb, and aPr values.
 * Output: An info2x2 struct object, with values assigned.
 */
info2x2 quantizeInfo2x2(float a, float b, float c, float d, float aPb,
                        float aPr)
{
    info2x2 pixInfo = (info2x2)malloc(sizeof(*pixInfo));
    assert(pixInfo);
    pixInfo->a = (unsigned)round(a * 511);
    pixInfo->b = findQuantized(b);
    pixInfo->c = findQuantized(c);
    pixInfo->d = findQuantized(d);
    pixInfo->aPb = Arith40_index_of_chroma(aPb);

    pixInfo->aPr = Arith40_index_of_chroma(aPr);
    return pixInfo;
}

/* findQuantized()
 * Purpose: Converts b, c, and d values to their quantized counterparts. If
 *          the given value is greater than 0.3 or less than -0.3, its
 *          quantized value is the same as 0.3/-0.3. Otherwise, the new
 *          quantized value is the original value, mulitplied by 50 and
 *          rounded.
 * Input: A number, representing an unquantized b, c, or d value
 * Output: The quantized value of b, c, or d
 */
int findQuantized(float num)
{
    if (num >= 0.3) {
        return 15;
    } else if (num <= -0.3) {
        return -15;
    }
    return round(num * 50);
}

/* setLSB()
 * Purpose: Assigns the widths/lsbs for a, b, c, d, aPb, and aPr values
 *          contained within a new lsbInfo struct object. The lsb values
 *          are passed in as parameters into the function, while the width
 *          values are predetermined by the spec.
 * Input: The lsb values for a, b, c, d, aPb, and aPr
 * Output: An lsbInfo struct object, with all of its values assigned.
 */
lsbInfo setLSB(unsigned a_lsb, unsigned b_lsb, unsigned c_lsb,
               unsigned d_lsb, unsigned aPb_lsb, unsigned aPr_lsb)
{
    lsbInfo formatInfo = (lsbInfo)malloc(sizeof(*formatInfo));
    formatInfo->a_lsb = a_lsb;
    formatInfo->b_lsb = b_lsb;
    formatInfo->c_lsb = c_lsb;
    formatInfo->d_lsb = d_lsb;
    formatInfo->aPb_lsb = aPb_lsb;
    formatInfo->aPr_lsb = aPr_lsb;

    formatInfo->a_w = 9;
    formatInfo->b_w = 5;
    formatInfo->c_w = 5;
    formatInfo->d_w = 5;
    formatInfo->aPb_w = 4;
    formatInfo->aPr_w = 4;

    return formatInfo;
}

/* packInfo2x2()
 * Purpose: Packs the quantized info from an info2x2 struct object into the
 *          first 32 bits of a 64-bit unsigned code word, by creating a new
 *          uint64_t object and inserting the a, b, c, d, aPb, and aPr values
 *          from the info2x2 object into their positions within the word,
 *          using Bitpack_newu and Bitpack_news.
 * Input: A filled info2x2 struct object, a filled lsbInfo struct object
 *        containing lsbs and widths for all variable in the info2x2 object.
 * Output: uint64_t code-word containing the quantized values from the info2x2
 *         struct at the positions determined by the lsbInfo format struct
 */
uint64_t packInfo2x2(info2x2 info, lsbInfo format)
{
    uint64_t word = 0;

    word = Bitpack_newu(word, format->a_w, format->a_lsb, (uint64_t)info->a);
    word = Bitpack_news(word, format->b_w, format->b_lsb, (uint64_t)info->b);
    word = Bitpack_news(word, format->c_w, format->c_lsb, (uint64_t)info->c);
    word = Bitpack_news(word, format->d_w, format->d_lsb, (uint64_t)info->d);
    word = Bitpack_newu(word, format->aPb_w,
                              format->aPb_lsb, (uint64_t)info->aPb);
    word = Bitpack_newu(word, format->aPr_w,
                              format->aPr_lsb, (uint64_t)info->aPr);
    return word;
}
