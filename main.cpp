#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <chrono>
#include <iostream>
#include <fstream>
#include "Image.h"
#include "PPM.h"
#include "MorphableOperators.h"

/*
 * Sperimentare con dimensioni diverse delle immagini.
 *
 * CUDA
 * Fissata una scheda video, giocare sul numero dei blocchi.
 * */

// TODO: implement for-each instead of for

int main() {

    const char *filename[4] = {"logitech_bill_clinton_bin.ppm", "micropro_wordstar_bin.ppm", "apple_adam_bin.ppm",
                               "two_bytes_better_bin.ppm"};
    double times[4 * 6];
    int times_index = 0;
    Image_t *input_img, *img, *out;
    std::string name;
    std::chrono::high_resolution_clock::time_point t_start, t_end;
    std::chrono::duration<double> time_span;

    for(int i = 0; i < 4; i+=1){
        input_img = PPM_import(filename[i]);
        printf("\nLoaded %s (%dx%d) \n", filename[i], input_img->width, input_img->height);

        // 0.0 -> BLACK ; 1.0 -> WHITE

        StructElem* se = new DiamondShape_SE(3);

        // Extract only the first channel.
        float *input = input_img->data;
        if(input_img->channels > 1) {
            input = (float *) malloc(input_img->width * input_img->height * sizeof(float));
            for (int r = 0; r < input_img->height; r += 1) {
                for (int c = 0; c < input_img->width; c += 1) {
                    input[r * input_img->width + c] = input_img->data[r * input_img->width * 3 + c * 3];
                }
            }
        }

        Image_t *img = Image_new(input_img->width, input_img->height, 1, input);


        // EROSION
        t_start = std::chrono::high_resolution_clock::now();
        out = erosion(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_eroded.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Erosion completed in %fs\n", times[times_index-1]);

        // DILATATION

        t_start = std::chrono::high_resolution_clock::now();
        out = erosion(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_dilatated.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Dilatation completed in %fs\n", times[times_index-1]);

        // CLOSING

        t_start = std::chrono::high_resolution_clock::now();
        out = closing(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_closed.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Closing completed in %fs\n", times[times_index-1]);

        // OPENING

        t_start = std::chrono::high_resolution_clock::now();
        out = opening(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_opened.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Opening completed in %fs\n", times[times_index-1]);

        // TOP HAT

        t_start = std::chrono::high_resolution_clock::now();
        out = top_hat(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_topHat.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Top Hat completed in %fs\n", times[times_index-1]);

        // BOTTOM HAT

        t_start = std::chrono::high_resolution_clock::now();
        out = bottom_hat(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        name = filename[i];
        PPM_export((name + "_bottomHat.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Bottom Hat completed in %fs\n", times[times_index-1]);

        // CLEAN AND GET READY FOR THE NEXT IMAGE

        Image_delete(input_img);
        Image_delete(out);
        free(input);

        printf("\n------ completed ------\n\n");

    }

    printf("\n\n####### Batch done #######");
    printf("\n\n => Writing results to file...");

    std::ofstream myfile;
    int k = 0;

    myfile.open ("Timings.csv");
    for(int i = 0; i < 4; i++){
        myfile << filename[i] << "\n";
        myfile << "EROSION;" << times[k++] << "\n";
        myfile << "DILATATION;" << times[k++] << "\n";
        myfile << "CLOSING;" << times[k++] << "\n";
        myfile << "OPENING;" << times[k++] << "\n";
        myfile << "TOPHAT;" << times[k++] << "\n";
        myfile << "BOTTOMHAT;" << times[k++] << "\n";
    }
    myfile.close();

    return 0;

}