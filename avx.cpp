#include "head.h"

/// @brief A logger function that prints contents of in var as hex
/// @param in input __m256i vector to output
/// @param ending_fmt how to end the line after printing the thing
void log (__m256i in, const char* ending_fmt = "") {

    int t[8] = {0};
    _mm256_maskstore_epi32 (t, _mm256_set1_epi8 (0xFF), in);    // Stores into array

    for (int i = 0; i < 8; i++) {

        printf ("%.2X%.2X%.2X%.2X", t[i] & 0xFF, (t[i] >> 8) & 0xFF, (t[i] >> 16) & 0xFF, (t[i] >> 24) & 0xFF); // Printfs bytewise cause otherwise every four are reversed
    }
    printf (ending_fmt);    // prints ending str
}

/// @brief The same as previous but float separated in underscores
/// @param in __m256 vector to be outputted
/// @param ending_fmt how to end the line after printing the thing
void logf (__m256 in, const char* ending_fmt = "") {

    float t[8] = {0};
    _mm256_maskstore_ps (t, _mm256_set1_epi8 (0xFF), in);   // Stores into array

    for (int i = 0; i < 8; i++) {

        printf ("%8f", t[i]);       // prints a float number
        if (i != 7) printf ("_");       // underscore to separate numbers
    }
    printf (ending_fmt);    // prints ending str
}

/// @brief Calculates whole screens specs and writes to rgba array
/// @param rgba array of pixels in rgbquad format
void calcScr (unsigned char* rgbaArray) {

    __m256 maxDistSq256 = _mm256_set1_ps (maxDistSq);   // max dist vector
    __m256 shift = _mm256_set1_ps (8 * (XLIMH - XLIML) / float (ScrSize));  // delta for each iteration of pixels
    __m256 x = _mm256_set_ps (7.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              6.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              5.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              4.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              3.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              2.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              1.0 / ScrSize * (XLIMH - XLIML) + XLIML,
                              0.0 / ScrSize * (XLIMH - XLIML) + XLIML); //x initial
    __m256 y = _mm256_set_ps (YLIMH - 7.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 6.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 5.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 4.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 3.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 2.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 1.0 / ScrSize * (YLIMH - YLIML),
                              YLIMH - 0.0 / ScrSize * (YLIMH - YLIML)); //y initial
    __m256 x00 = x; //saving initial x

    int cnt = 0;
    for (int scrIter = 0; scrIter < ScrSize * ScrSize * 4; scrIter += 4 * 8) { // iterates in 8 pixel wide parts

        if (cnt >= ScrSize) {   //  if end of line
            x = x00;    // reset x
            y = _mm256_sub_ps (y, shift);// shift y
            cnt = 0;// zero cnt
        }

        __m256 x0 = x;  // saves start coords
        __m256 y0 = y;  // saves start coords

        __m256i iRes = _mm256_setzero_si256 ();

        for (int i = 0 ; i < topCalcLimit; i++) {

            __m256 x2 = _mm256_mul_ps (x, x);   // squared
            __m256 y2 = _mm256_mul_ps (y, y);   // squared

            __m256 mask = _mm256_cmp_ps (_mm256_add_ps (x2, y2), maxDistSq256, _CMP_LE_OQ); // mask[i] = 0xFFFFFFFF if further calculation needed for point, 0x0 otherwise
            __m256i imask = _mm256_castps_si256 (mask); // the same but __m256i

            iRes = _mm256_add_epi32 (_mm256_and_si256 (imask, _mm256_sub_epi8 (_mm256_set1_epi32 (i) , iRes)), iRes);

            if (_mm256_movemask_epi8 (imask) == 0) break; // if no calcs needed (all == 0x0) then break

            y = _mm256_add_ps (_mm256_and_ps (mask, _mm256_sub_ps (     // condition to not calc ones that are zeroed in mask
                _mm256_add_ps (_mm256_mul_ps (_mm256_set1_ps (2.0), _mm256_mul_ps (x, y)), y0), // expression
                y)), y); // end of condition
            x = _mm256_add_ps (_mm256_and_ps (mask, _mm256_sub_ps (
                _mm256_add_ps (_mm256_sub_ps (x2, y2), x0),             // the same as 2 lines up
                x)), x);
        }

        x =_mm256_add_ps (x0, shift);   // shifts x cause its faster than recalculating every time
        y = y0; // resets y
        cnt+=8; // adds cnt

        iRes = _mm256_and_si256 (iRes, _mm256_cmpeq_epi32 (iRes, _mm256_set1_epi32 (topCalcLimit))); //sets iRes[i] = 0 if iRes[i] == topCalcLimit

        int iResInt[8] = {0};
        _mm256_maskstore_epi32 (iResInt, _mm256_set1_epi32 (-1), iRes); //  stores iRes to an array because its way easier to
                                            // do all the operations with array elems than with 256 fucking vectors
                                            // and because loop here has a fixed length of 8
                                            // it does not impact the performance if
                                            // ScrSize is like more than 64

        for (int i = 0; i < 8; i++) {

            rgbaArray[scrIter + i + 0] = r0 + (iResInt[i] * (r1 - r0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + i + 1] = g0 + (iResInt[i] * (g1 - g0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + i + 2] = b0 + (iResInt[i] * (b1 - b0) / topCalcLimit);  // Formats i into color with alpha
            rgbaArray[scrIter + i + 3] = a0 + (iResInt[i] * (a1 - a0) / topCalcLimit);  // Formats i into color with alpha
        }
    }

}