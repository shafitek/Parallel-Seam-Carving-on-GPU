#ifndef SHAFITEK_SEAM_CARVER_H
#define SHAFITEK_SEAM_CARVER_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <opencv2/opencv.hpp>

class SeamCarver
{
    public:
        SeamCarver(const char *, const bool &);
        cv::Mat carveImage(int resize_factor);
        double getExecutionTime() const;

    protected:
        cv::Mat getImage() const;
        const bool getIfReduce() const;
        void setExecutionTime(double);

    private:
        cv::Mat img;
        bool reduce;
        double exec_time;
        virtual cv::Mat getEnergyMap() = 0;
        virtual cv::Mat getOptimalSeam() = 0;
};

#endif // SHAFITEK_SEAM_CARVER_H