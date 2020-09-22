#include "GPUCarver.h"

GPUCarver::GPUCarver(const char *img_path, const bool &reduce) : SeamCarver(img_path, reduce)
{
}

cv::Mat GPUCarver::getEnergyMap() {
    cv::Mat res;
    return res;
}
cv::Mat GPUCarver::getOptimalSeam() {
    cv::Mat res;

    return res;
}
