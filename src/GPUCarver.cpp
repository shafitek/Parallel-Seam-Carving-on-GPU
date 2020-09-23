#include "GPUCarver.h"

GPUCarver::GPUCarver(const char *img_path, const bool &reduce) : SeamCarver(img_path, reduce)
{
}

void GPUCarver::computeEnergyMap() {
    cv::Mat res;
    auto start = std::chrono::high_resolution_clock::now();
    for (auto i = 0; i < 1000000000; i++);
    auto end = std::chrono::high_resolution_clock::now();
    double diff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    setExecutionTime(diff);
}
void GPUCarver::computeOptimalSeam() {
    cv::Mat res;

}
