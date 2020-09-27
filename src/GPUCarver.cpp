#include "GPUCarver.h"

GPUCarver::GPUCarver(const char *img_path, const bool &reduce) : SeamCarver(img_path, reduce)
{}
void GPUCarver::computeEnergyImage()
{
}

void GPUCarver::computeEnergyMap() {
    cv::Mat res;
    double time;
    computeEnergyMapGPU(getImage(), this->energy_img, this->energy_map, time);
    setExecutionTime(time);
}
