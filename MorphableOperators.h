#ifndef MORPHOPSOPENMP_MORPHABLEOPERATORS_H
#define MORPHOPSOPENMP_MORPHABLEOPERATORS_H

#endif //MORPHOPSOPENMP_MORPHABLEOPERATORS_H

#include "StructuringElement.h"
#include "Image.h"

// TODO: wrap in a static class

#define EROSION 1
#define DILATATION 0

Image_t* erosion(Image_t* input, StructElem* structElem);
Image_t* dilatation(Image_t* input, StructElem* structElem);
Image_t * opening(Image_t* input, StructElem* structElem);
Image_t * closing(Image_t* input, StructElem* structElem);
Image_t * top_hat(Image_t* input, StructElem* structElem);
Image_t * bottom_hat(Image_t* input, StructElem* structElem);
Image_t * process(Image_t* input, StructElem* structElem, int operation);

float max_ar(float* source, int len);
float min_ar(float* source, int len);
int complOpt(int op);
template<typename T>
static inline T _abs(const T &a) {
    return a < 0 ? -a : a;
}