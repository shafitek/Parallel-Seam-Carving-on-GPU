#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include <unistd.h>

#include "SeamCarver.h"
#include "CPUCarver.h"
#include "GPUCarver.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    std::cout << "Parallel Seam Carving on the GPU" << std::endl;
    std::cout << "--------------------------------\n" << std::endl;

    if(argc < 5) {
        std::cout << "Usage: ./SeamCarving <Image_Path> <Runner> <Resize> <Resize_Factor>" << std::endl;
        std::cout << "       <Image_Path> -> Absolute/Relative path to the image" << std::endl;
        std::cout << "       <Runner> - CPU | GPU | BOTH" << std::endl;
        std::cout << "       <Resize> - Decrease = 0, Increase = 1" << std::endl;
        std::cout << "       <Resize Factor> - # of pixels to reduce horizontally\n" << std::endl;
        std::cout << "Example: ./SeamCarving ../images/lake.jpg BOTH 0 100\n" << std::endl;
        return -1;
    }

    const char *img_path = argv[1];
    const char *run_type = argv[2];
    const bool reduce = std::stoi(argv[3]);
    const unsigned int resize_factor = std::stoi(argv[4]);

    cv::Mat A;
    A = cv::imread(img_path, -1);

    if( !A.data ) {
        printf("Invalid image.\n");
        return -1;
    }

    int run_type_i = ifCPUGPUBoth(run_type);
    std::string run_type_s = run_type_i == 0 ? "CPU" : run_type_i == 1 ? "GPU" : "CPU, GPU";
    std::string reduce_s = reduce ? "Increase" : "Decrease";

    std::cout << "DETAILS:" << std::endl;
    std::cout << "Image Path: " << img_path << std::endl;
    std::cout << "Resolution: " << A.rows << " x " << A.cols << " pixels" << std::endl;
    std::cout << "Runner: " << run_type_s << std::endl << std::endl;
    std::cout << "-> GOAL: " << reduce_s << " image by " << resize_factor << "px horizontally <-\n" << std::endl;

    double cpu_time=0.0, gpu_time=0.0;

    SeamCarver *seamCarver = nullptr;
    CPUCarver cpuCarver = CPUCarver(img_path, reduce);
    GPUCarver gpuCarver = GPUCarver(img_path, reduce);

    if (run_type_i == 0 || run_type_i == 2) // CPU
    {
        std::cout << "Carving via the CPU. Please wait..." << std::flush;
        seamCarver = &cpuCarver;
        seamCarver->carveImage(resize_factor);
        cpu_time = seamCarver->getExecutionTime();
        std::cout << "CPU carving completed!" << std::endl;
        // extra space included after "ms" because the function does not fully clear the output
        std::cout << "\nCPU Execution Time: " << cpu_time << " ms" << std::endl;
        cv::namedWindow("CPU", cv::WINDOW_AUTOSIZE);
        cv::imshow("CPU", seamCarver->getCPUCarvedImage());
    }

    if (run_type_i == 1 || run_type_i == 2) // GPU
    {
        seamCarver = &gpuCarver;
        std::cout << "Carving via the GPU. Please wait..." << std::flush;
        seamCarver->carveImage(resize_factor);
        gpu_time = seamCarver->getExecutionTime();
        eraseTextFromTerminal(35);
        std::cout << "GPU Execution Time: " << gpu_time << " ms        " << std::endl;
    }

    std::cout << "\nFinished!" << std::endl;

    cv::namedWindow("Original", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original", seamCarver->getOrigImage());
    cv::waitKey(0);

    printf("\n");

    return 0;
}
