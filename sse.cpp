#include "head.h"

/// @brief Calculates whole screens specs and writes to rgba array
/// @param rgba array of pixels in rgbquad format
void calcScr (unsigned char* rgba) {

    __m256 maxDistSq256 = _mm256_set1_ps (maxDistSq);   // max dist vector
    __m256 shift = _mm256_set1_ps (8 * (XLIMH - XLIML) / ScrSize);  // delta for each iteration of pixels
    __m256 x = _mm256_sub_ps (_mm256_add_ps (_mm256_set_ps (0, 4, 8, 12, 16, 20, 24, 28), _mm256_set1_ps (XLIML)), shift); //x initial
    __m256 y = _mm256_sub_ps (_mm256_set1_ps (YLIMH), _mm256_set_ps (0, 4, 8, 12, 16, 20, 24, 28)); //y initial
    __m256 x00 = _mm256_add_ps (x, shift);//saving initial x

    for (int scrIter = 0, cnt = 0; scrIter < ScrSize * ScrSize * 4; scrIter += 4 * 8, cnt++) { // iterates in 8 pixel wide parts

        if (cnt == ScrSize) {   //  if end of line
            x = x00;    // reset x
            y = _mm256_sub_ps (y, shift);// shift y
            cnt = 0;// zero cnt
        }
        else x = _mm256_add_ps (x, shift);//shift x

        __m256 x0 = x;  // saves start coords
        __m256 y0 = y;  // saves start coords

        for (int i = 0 ; i < topCalcLimit; i++) {

            __m256 x2 = _mm256_mul_ps (x, x);   // squared
            __m256 y2 = _mm256_mul_ps (y, y);   // squared

            __m256 mask = _mm256_cmp_ps (_mm256_add_ps (x2, y2), maxDistSq256, _CMP_LE_OQ); // mask[i] = 0xFFFFFFFF if further calculation needed for point, 0x0 otherwise

            if (_mm256_movemask_ps (mask) == 0) break; // if no calcs needed (all == 0x0) then break

            y = _mm256_add_ps (
                y,
                _mm256_and_ps (
                    mask,
                    _mm256_sub_ps (
                        _mm256_add_ps (
                            y0,
                            _mm256_mul_ps (
                                _mm256_set1_ps (2.0),
                                _mm256_mul_ps (
                                    x,
                                    y
                                )
                            )
                        ),
                    y)
                )
            );
            // y[i] = (mask[i] == 0 ? 0 : 2 * x[i] * y[i] + y0[i] - y[i]) + y[i]; i in range (0, 8);
            x = _mm256_add_ps (
                x,
                _mm256_and_ps (
                    mask,
                    _mm256_sub_ps (
                        _mm256_add_ps (
                            x2,
                            _mm256_sub_ps (
                                x0,
                                y2
                            )
                        ),
                        x
                    )
                )
            );
            // x[i] = (mask[i] == 0 ? 0 : x2[i] + y2[i] + x0[i] - x[i]) + x[i]; i in range (0, 8)
        }

    }
}