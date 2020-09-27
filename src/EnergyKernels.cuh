#ifndef SHAFITEK_ENERGY_KERNELS_H
#define SHAFITEK_ENERGY_KERNELS_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

void computeEnergyMapGPU(const cv::Mat &, cv::Mat &, cv::Mat &, double &);

#endif // SHAFITEK_ENERGY_KERNELS_H