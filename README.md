# MorphOpsCPP
Matematical morphological operators implementation in C++11

## Overview
A simple implementation of morfological operators, the code is used to compare the CUDA implementation ([here](https://github.com/rickie95/MorphOpsCUDA)) and measure speedup.

Code works with .ppm B/W images, extracting only the first channel if multiple channels are detected. Results are also .ppm images, timings are always saved in a .csv file.

Regarding operators, multiple shapes are avaiable: you can even set a custom sizes. Check out [StructuringElement.h](https://github.com/rickie95/MorphOpsCPP/blob/master/StructuringElement.h).

[MorphableOperators.h](https://github.com/rickie95/MorphOpsCPP/blob/master/MorphableOperators.h) contains implementations of some well know tranformations: erosion, dilatation, opening, closing, top-hat and bottom-hat transform. Derived transformations were implemented calling base transformations methods.
