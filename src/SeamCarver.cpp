#include "SeamCarver.h"

SeamCarver::SeamCarver(const char *img_path, const bool &reduce)
{
    this->img = cv::imread(img_path, -1);
    this->reduce = reduce;
}

cv::Mat SeamCarver::carveImage(int resize_factor)
{
    cv::Mat res = this->img.clone();
    getEnergyMap();
    return res;
}

cv::Mat SeamCarver::getImage() const
{
    return this->img;
}

const bool SeamCarver::getIfReduce() const {
    return this->reduce;
}

double SeamCarver::getExecutionTime() const
{
    return this->exec_time;
}

void SeamCarver::setExecutionTime(double time)
{
    this->exec_time = time;
}
