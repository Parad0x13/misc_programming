// If this is the first time main.h has been used withing the program then use the following code
#ifndef FRACTREF_H
#define FRACTREF_H

/* Different types of fractals supported */
#define FRACT_MANDELBROT 1
#define FRACT_JULIA 2
#define FRACT_MANDELBAR 3

#define FRACT_HENON 4
#define FRACT_TINKERBELL 5
#define FRACT_LORENZ 6
#define FRACT_RABINOVICHFABRIKANT 7
#define FRACT_HADLEY 8
#define FRACT_ROSSLER 9
#define FRACT_ACT 10
#define FRACT_DUFFING 11
#define FRACT_PICKOVER 12
#define FRACT_IKEDA 13
#define FRACT_RUCKLIDGE 14

#define FRACT_IFS_SIERPINSKI 801
#define FRACT_IFS_TRIANGLES 802
#define FRACT_IFS_PENTAGON 803

#define TEST_MANDELBROT 804

#define FRACT_BURNINGSHIP 999
#define FRACT_BUDDAHBROT 998

/* Different types of shading algorithms supported */
#define NORMALIZE_1 1
#define NORMALIZE_2 2
#define ESCAPETIME 3
/* Variations */
#define NORMALIZE_1_V1 4
#define NORMALIZE_1_V2 5
#define NORMALIZE_1_V3 6
#define NORMALIZE_1_V4 7

#endif		// End the ifndef
