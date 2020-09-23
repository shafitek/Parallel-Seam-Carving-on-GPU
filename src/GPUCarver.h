#ifndef SHAFITEK_GPU_CARVER_H
#define SHAFITEK_GPU_CARVER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "SeamCarver.h"

class GPUCarver: public SeamCarver {
    public:
        GPUCarver(const char *, const bool &);

    private:
        cv::Mat energy_map;
        void computeEnergyMap();
        void computeOptimalSeam();
};

#endif // SHAFITEK_GPU_CARVER_H