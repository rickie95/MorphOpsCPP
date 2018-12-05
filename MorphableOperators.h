#ifndef MORPHOPSOPENMP_MORPHABLEOPERATORS_H
#define MORPHOPSOPENMP_MORPHABLEOPERATORS_H
#endif //MORPHOPSOPENMP_MORPHABLEOPERATORS_H

#include "StructuringElement.h"
#include "Image.h"

#define EROSION 1
#define DILATATION 0

class MorphableOperator{

private:
    static Image_t * process(Image_t* input, StructElem* structElem, int operation);

public:
    static Image_t* erosion(Image_t* input, StructElem* structElem);
    static Image_t* dilatation(Image_t* input, StructElem* structElem);
    static Image_t * opening(Image_t* input, StructElem* structElem);
    static Image_t * closing(Image_t* input, StructElem* structElem);
    static Image_t * top_hat(Image_t* input, StructElem* structElem);
    static Image_t * bottom_hat(Image_t* input, StructElem* structElem);

};

float max_ar(float* source, int len);
float min_ar(float* source, int len);
int complOpt(int op);
template<typename T>
static inline T _abs(const T &a) {
    return a < 0 ? -a : a;
}