#include "MorphableOperators.h"
#include <cstdlib>

Image_t* MorphableOperator::erosion(Image_t* input, StructElem* structElem){
    return process(input, structElem, EROSION);
}

Image_t* MorphableOperator::dilatation(Image_t* input, StructElem* structElem){
    return process(input, structElem, DILATATION);
}

Image_t* MorphableOperator::opening(Image_t* input, StructElem* structElem){ // EROSION then DILATATION
    return process(process(input, structElem, EROSION), structElem, DILATATION);
}

Image_t* MorphableOperator::closing(Image_t* input, StructElem* structElem) { // DILATATION then EROSION
    return process(process(input, structElem, DILATATION), structElem, EROSION);
}

Image_t * MorphableOperator::top_hat(Image_t* input, StructElem* structElem){ // ORIGINAL - OPENED
    Image_t* opened = opening(input, structElem);
    float* output = (float*)malloc(input->width * input->height * sizeof(float));
    for(long i = 0; i < input->width * input->height; i+=1){
        output[i] = _abs(input->data[i] - opened->data[i]);
    }
    return Image_new(input->width, input->height, 1, output);
}

Image_t * MorphableOperator::bottom_hat(Image_t* input, StructElem* structElem){ // CLOSED - ORIGINAL
    Image_t* closed = closing(input, structElem);
    float* output = (float*)malloc(input->width * input->height * sizeof(float));
    for(long i = 0; i < input->width * input->height; i+=1){
        output[i] = _abs(closed->data[i] - input->data[i]);
    }
    return Image_new(input->width, input->height, 1, output);
}

Image_t* MorphableOperator::process(Image_t *input_img, StructElem *elem, int operation){ //float (*operation)(float*, int) ){

    // Assumes that input_img->data is a NxM float matrix with values 0 or 1
    // OPERATION: 1 if erosion, 0 if dilatation

    short int elem_RY = elem->get_radius_y();
    short int elem_RX = elem->get_radius_x();
    short int elem_W = elem->get_width();
    short int elem_H = elem->get_height();
    float *struc_elem = elem->data;

    float *input = input_img->data;
    float *output = (float *) malloc(input_img->width * input_img->height * sizeof(float));

    for (int r = 0; r < input_img->height; r += 1) {
        for (int c = 0; c < input_img->width; c += 1)
            output[r * input_img->width + c] = 0;
    }

    for (int row = 0; row < input_img->height; row += 1) {
        for (int col = 0; col < input_img->width; col += 1) {
            // Extract neighborhood using mask as reference
            int k = 0;
            float neighborhood[elem_H * elem_W]; // at least all the mask
            for (int i = 0; i < elem_H; i += 1) {
                for (int j = 0; j < elem_W; j += 1) {
                    int x = (row + i - elem_RX);
                    int y = (col + j - elem_RY);
                    if (x > -1 && x < input_img->height && y > -1 && y < input_img->width &&
                        struc_elem[i * elem_W + j] > 0) {
                        neighborhood[k] = input[x * input_img->width + y];
                        k += 1;
                    }
                }
            }
            if (operation == EROSION) // EROSION
                output[row * input_img->width + col] = max_ar(neighborhood, k);

            if (operation == DILATATION) // DILATATION
                output[row * input_img->width + col] = min_ar(neighborhood, k);
        }
    }
    return Image_new(input_img->width, input_img->height, 1, output);
}

float max_ar(float* source, int len){
    float max = source[0];
    for(int i = 0; i < len; i++){
        if(source[i] > max)
            max = source[i];
    }
    return max;
}

float min_ar(float* source, int len){
    float min = source[0];
    for(int i = 0; i < len; i++){
        if(source[i] < min)
            min = source[i];
    }
    return min;
}

int complOpt(int op){
    return op==1 ? 0 : 1;
}

