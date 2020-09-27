#ifndef SHAFITEK_GPU_CARVER_H
#define SHAFITEK_GPU_CARVER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "SeamCarver.h"
#include "EnergyKernels.cuh"

class GPUCarver: public SeamCarver {
    public:
        GPUCarver(const char *, const bool &);

    private:
        void computeEnergyImage();
        void computeEnergyMap();
};

#endif // SHAFITEK_GPU_CARVER_H