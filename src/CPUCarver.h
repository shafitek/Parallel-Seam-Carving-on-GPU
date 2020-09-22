#ifndef SHAFITEK_CPU_CARVER_H
#define SHAFITEK_CPU_CARVER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "SeamCarver.h"

class CPUCarver: public SeamCarver {
    public:
        CPUCarver(const char *, const bool &);

    private:
        cv::Mat energy_map;
        cv::Mat getEnergyMap();
        cv::Mat getOptimalSeam();
};

#endif // SHAFITEK_CPU_CARVER_H