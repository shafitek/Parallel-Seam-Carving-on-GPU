#ifndef SHAFITEK_SEAM_CARVER_H
#define SHAFITEK_SEAM_CARVER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <utility>
#include <opencv2/opencv.hpp>

#include "utils.h"

class SeamCarver
{
    public:
        SeamCarver(const char *, const bool &);
        void carveImage(const unsigned int &);
        double getExecutionTime() const;
        std::pair<int, int> getOrigDim() const;
        std::pair<int, int> getCarvedDim() const;
        cv::Mat getOrigImage() const;
        cv::Mat getCPUCarvedImage() const;
        cv::Mat getGPUCarvedImage() const;

    protected:
        cv::Mat energy_img;
        cv::Mat energy_map;
        cv::Mat getImage() const;
        const bool getIfReduce() const;
        void setOptimalSeam(std::vector<unsigned int> &);
        void setExecutionTime(double &);

    private:
        cv::Mat orig_img;
        cv::Mat cpu_img, gpu_img;
        std::vector<unsigned int> optimal_seam;
        bool reduce;
        double exec_time=0.0;
        std::pair<int,int> orig_dim, curr_dim;
        virtual void computeEnergyMap() = 0;
        virtual void computeOptimalSeam() = 0;
};

#endif // SHAFITEK_SEAM_CARVER_H