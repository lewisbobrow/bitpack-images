/*
 *  HW4: Machine Arithmetic
 *  bitpack.c
 *  Authors: Lewis Bobrow (lbobro01) and JJ Stewart (jstewa12)
 *
 */

#include <stdio.h>
#include <inttypes.h>
#include "bitpack.h"
#include "assert.h"
#include "math.h"


/* Bitpack_fitsu()
 * Purpose: Checks whether a given unsigned value can fit into a given number
            of bits, by shifting the value to the left and the right of the
            given range of bits and checking if its original value is intact.
 * Input: A uint64_t object, representing an unsigned value; a certain number
          of bits the value may or may not be able to fit into, which cannot
          be greater than 64 bits.
 * Output: A true/false value for whether or not the given value can fit.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64);
    uint64_t temp = n;
    temp = temp << (64 - width) >> (64 - width);
    if (temp == n) {
        return true;
    }
    return false;
}


/* Bitpack_fitss()
 * Purpose: Checks whether a given signed value can fit into a given number
            of bits, by shifting the value to the left and the right of the
            given range of bits and checking if its original value and sign
            are intact.
 * Input: An int64_t object, representing a signed value; a certain number
          of bits the value may or may not be able to fit into, which cannot
          be greater than 64 bits.
 * Output: A true/false value for whether or not the given value can fit.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64);
    int64_t temp = n;
    temp = temp << (64 - width) >> (64 - width);
    if (temp == n) {
        return true;
    }
    return false;
}


/* Bitpack_getu()
 * Purpose: To retrieve a certain unsigned value (starting at a certain lsb
            and contained within a given number of bits) from a given word.
 * Input: A given word, the number of bits a certain value is contained in,
          the least significant bit where the definition of the value begins.
          The width cannot be longer than 64 bits (the length of the word),
          as can't the width + lsb.
 * Output: The value contained with the word
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
    uint64_t temp = word;

    /* gets rid of stray bits before needed value, shifts value to
       beginning of word */
    temp = temp << (64 - (width + lsb)) >> (64 - width);
    return temp;
}


/* Bitpack_gets()
 * Purpose: To retrieve a certain signed value (starting at a certain lsb and
            contained within a given number of bits) from a given word.
 * Input: A given word, the number of bits a certain value is contained in,
          the least significant bit where the definition of the value begins.
          The width cannot be longer than 64 bits (the length of the word),
          as can't the width + lsb.
 * Output: The value contained with the word
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
    assert(width <= 64);
    assert(width + lsb <= 64);
    int64_t temp = word;

    /* gets rid of stray bits before needed value (maintains signage)
       shifts value to beginning of word */
    temp = temp << (64 - (width + lsb)) >> (64 - width);
    return temp;
}


/* Bitpack_newu()
 * Purpose: To update the unsigned value contained at a certain position in a
            given word, by first creating a mask which will extract the
            original value from the word, using the mask to create a new word
            that doesn't have the original value, inserting the new value into
            the new word, and returning the new word.
 * Input: A given word, the number of bits the original value is contained in,
          the least significant bit where the definition of the original value
          begins, and a new value to insert in place of the original value.
          width + lsb can't be more than 64 bits long, and the new unsigned
          value must fit into the bit-width allotted for the original value.
 * Output: A new word with the udpated value contained in it.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value)
{
    assert((width + lsb) <= 64);
    assert(Bitpack_fitsu(value, width) == true);
    uint64_t zero = 0;
    uint64_t mask = (~zero) << (64 - (width + lsb));
    mask = mask >> (64 - width) << lsb;
    uint64_t new_word = (~mask) & word;
    value = value << lsb;
    new_word = new_word | value;
    return new_word;
}


/* Bitpack_news()
 * Purpose: To update the signed value contained a certain position in a
            given word, by first assuring the new signed value and its sign
            fit within the given bits of the original value, then creating a
            mask which will extract the original value from the word, using
            the mask to create a new word that doesn't have the original
            value, inserting the new value into the new word, and returning
            the new word.
 * Input: A given word, the number of bits the original value is contained in,
          the least significant bit where the definition of the original value
          begins, and a new value to insert in place of the original value.
          width + lsb can't be more than 64 bits long, and the new value must
          fit into the width allotted for the original value.
 * Output: A new word with the udpated value contained in it.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value)
{
    assert((width + lsb) <= 64);
    assert(Bitpack_fitss(value, width) == true);
    uint64_t zero = 0;
    uint64_t mask = (~zero) << (64 - (width + lsb));
    mask = mask >> (64 - width) << lsb;
    uint64_t new_word = (~mask) & word;
    uint64_t valMask = pow(2, width) - 1;
    valMask = (valMask & value) << lsb;
    new_word = new_word | valMask;
    return new_word;
}
