#include "head.h"

/// @brief Runs calculations for point (x,y) and writes results to *{r, g, b, a} in unsigned char array
/// @param r ptr to red
/// @param g ptr to green
/// @param b ptr to blue
/// @param a ptr to alpha
/// @param x point coord
/// @param y point coord
void calcPoint (unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a, float x, float y) {

    int i = 0;  // iter count

    float x_0 = x;  // saves initial pos
    float y_0 = y;  // saves initial pos


    for (i = 0; i < topCalcLimit; i++) {

        float x_2 = x * x;  // x, y squared
        float y_2 = y * y;  // x, y squared

        if (x_2 + y_2 >= maxDistSq) break;
        float xt = x;
        x = x_2 - y_2 + x_0;    // calculation of new iteration
        y = 2 * xt * y + y_0;       // calculation of new iteration

    }

    if (i == topCalcLimit) i = 0; // sets to default color if maxxed out the calculation limit
    *r = r0 + (i * (r1 - r0) / topCalcLimit); //
    *g = g0 + (i * (g1 - g0) / topCalcLimit); // setting color calculated as a simple fader (could be made logarithmical for fun)
    *b = b0 + (i * (b1 - b0) / topCalcLimit); //

    *a = 255;
}