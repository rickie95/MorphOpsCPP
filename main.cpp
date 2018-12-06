#include <cstdlib>
#include <cstdio>
#include <cstdio>
#include <chrono>
#include <fstream>
#include <vector>
#include "Image.h"
#include "PPM.h"
#include "MorphableOperators.h"

int main() {

    std::vector<std::string> *filename = new std::vector<std::string>();
    filename->push_back("logitech_bill_clinton_bin.ppm");
    filename->push_back("micropro_wordstar_bin.ppm");
    filename->push_back("apple_adam_bin.ppm");
    filename->push_back("two_bytes_better_bin.ppm");

    double times[filename->size() * 6];
    int times_index = 0;
    Image_t *input_img, *img, *out;
    std::string name;
    std::chrono::high_resolution_clock::time_point t_start, t_end;
    std::chrono::duration<double> time_span;

    for(auto file = filename->begin(); file != filename->end(); ++file){
        input_img = PPM_import(file->c_str());
        printf("\nLoaded %s (%dx%d) \n", file->c_str(), input_img->width, input_img->height);

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

        img = Image_new(input_img->width, input_img->height, 1, input);


        // EROSION
        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::erosion(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_eroded.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Erosion completed in %fs\n", times[times_index-1]);

        // DILATATION

        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::dilatation(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_dilatated.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Dilatation completed in %fs\n", times[times_index-1]);

        // CLOSING

        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::closing(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_closed.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Closing completed in %fs\n", times[times_index-1]);

        // OPENING

        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::opening(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_opened.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Opening completed in %fs\n", times[times_index-1]);

        // TOP HAT

        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::top_hat(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_topHat.ppm").c_str(), out);

        time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t_end - t_start);
        times[times_index++] = time_span.count();

        printf("Top Hat completed in %fs\n", times[times_index-1]);

        // BOTTOM HAT

        t_start = std::chrono::high_resolution_clock::now();
        out = MorphableOperator::bottom_hat(img, se);
        t_end = std::chrono::high_resolution_clock::now();

        PPM_export((*file + "_bottomHat.ppm").c_str(), out);

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
    for(auto file = filename->begin(); file != filename->end(); ++file){
        myfile << file->c_str() << "\n";
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