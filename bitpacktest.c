#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "assert.h"
#include "math.h"
#include "pnm.h"
#include "bitpack.h"

int main() {
    // Testing fitsu
    uint64_t utest_fits = 15;
    if (Bitpack_fitsu(utest_fits, 5)) {
        printf("15 can fit in 5 bits.\n");
    } else {
	    printf("15 CANNOT fit in 5 bits!\n");
    }

    if (Bitpack_fitsu(utest_fits, 4)) {
        printf("15 can fit in 4 bits.\n");
    } else {
	    printf("15 CANNOT fit in 4 bits!\n");
    }

    if (Bitpack_fitsu(utest_fits, 3)) {
        printf("15 can fit in 3 bits.\n");
    } else {
	    printf("15 CANNOT fit in 3 bits!\n");
    }


    // Testing fitss with a positive number
    int64_t stest_fits = 16;
    if (Bitpack_fitss(stest_fits, 4) == true) {
        printf("16 can fit in 4 bits.\n");
    } else {
        printf("16 CANNOT fit in 4 bits.\n");
    }

    if (Bitpack_fitss(stest_fits, 5) == true) {
        printf("16 can fit in 5 bits.\n");
    } else {
	       printf("16 CANNOT fit in 5 bits.\n");
    }

    if (Bitpack_fitss(stest_fits, 6) == true) {
        printf("16 can fit in 6 bits.\n");
    } else {
        printf("16 CANNOT fit in 6 bits.\n");
    }


    // Testing fitss with a negative number
    int64_t stest2_fits = -16;
    if (Bitpack_fitss(stest2_fits, 4) == true) {
        printf("-16 can fit in 4 bits.\n");
    } else {
        printf("-16 CANNOT fit in 4 bits.\n");
    }

    if (Bitpack_fitss(stest2_fits, 5) == true) {
        printf("-16 can fit in 5 bits.\n");
    } else {
        printf("-16 CANNOT fit in 5 bits.\n");
    }

    if (Bitpack_fitss(stest2_fits, 6) == true) {
        printf("-16 can fit in 6 bits.\n");
    } else {
        printf("-16 CANNOT fit in 6 bits.\n");
    }



    // Testing getu
    uint64_t utest_get = 2479;
    if (Bitpack_getu(utest_get, 4, 4) == 10) {
        printf("Successfully got 10\n");
    } else {
	    printf("Unsuccessful!\n");
    }


    // Testing gets, getting a positive number from a positive number
    int64_t stest_get = 543245;
    if (Bitpack_gets(stest_get, 7, 9) == 37) {
        printf("Successfully got 37\n");
    } else {
	    printf("Unsuccessful!\n");
    }


    // Testing gets, getting a negative number from a positive number
    int64_t stest2_get = 543245;
    if (Bitpack_gets(stest2_get, 6, 9) == -27) {
        printf("Successfully got -27\n");
    } else {
	    printf("Unsuccessful!\n");
    }


    // Testing gets, getting a negative number from a positive number
    int64_t stest3_get = -6789;
    if (Bitpack_gets(stest3_get, 4, 8) == 5) {
        printf("Successfully got 5\n");
    } else {
	    printf("Unsuccessful!\n");
    }


    // Testing gets, getting a negative number from a negative number
    int64_t stest4_get = -6789;
    if (Bitpack_gets(stest4_get, 3, 8) == -3) {
        printf("Successfully got -3\n");
    } else {
	    printf("Unsuccessful!\n");
    }



    // Testing newu
    uint64_t utest_new = 2479;
    if (Bitpack_newu(utest_new, 4, 4, 15) == 2559) {
        printf("Successfully added 15\n");
    } else {
        printf("Unsuccessful!\n");
    }


    // Testing news
    int64_t stest_new = 543245;
    if (Bitpack_news(stest_new, 7, 9, 38) == 543757) {
        printf("Successfully added 38\n\n");
    } else {
        printf("Unsuccessful!\n\n");
    }


    // Testing news with a positive number
    int64_t stest2_new = 543245;
    if (Bitpack_news(stest2_new, 6, 9, -26) == 543757) {
        printf("Successfully added -26\n\n");
    } else {
        printf("Unsuccessful!\n\n");
    }
}
