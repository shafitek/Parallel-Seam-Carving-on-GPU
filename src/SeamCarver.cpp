#include "SeamCarver.h"

SeamCarver::SeamCarver(const char *img_path, const bool &reduce)
{
    this->cpu_img = cv::imread(img_path, cv::IMREAD_COLOR);
    this->orig_img = this->cpu_img.clone();
    this->reduce = reduce;

    this->orig_dim = std::make_pair(this->orig_img.rows, this->orig_img.cols);
    this->curr_dim = std::make_pair(this->orig_img.rows, this->orig_img.cols);
}

void SeamCarver::carveImage(const unsigned int &resize_factor)
{
    for(int m = 0; m < resize_factor; m++) {
        cv::Mat img = this->cpu_img;
        computeEnergyMap();
        computeOptimalSeam();

        cv::Mat n_b = cv::Mat::zeros(img.rows, img.cols - 1, CV_8U);
        cv::Mat n_g = cv::Mat::zeros(img.rows, img.cols - 1, CV_8U);
        cv::Mat n_r = cv::Mat::zeros(img.rows, img.cols - 1, CV_8U);

        std::vector<cv::Mat> new_img_channel = {n_b, n_g, n_r};

        std::vector<unsigned int> &seam = this->optimal_seam;

        cv::Mat chb, chg, chr;
        std::vector<cv::Mat> channels;
        cv::split(img, channels);

        chb = channels[0];
        chg = channels[1];
        chr = channels[2];

        int offset;
        for (int i = 0; i < img.rows; i++)
        {
            offset = 0;
            for (int j = 0; j < img.cols; j++)
            {
                if (j == seam[i]) {
                    offset = -1;
                    continue;
                }
                n_b.at<unsigned char>(i, j+offset) = chb.at<unsigned char>(i, j);
                n_g.at<unsigned char>(i, j+offset) = chg.at<unsigned char>(i, j);
                n_r.at<unsigned char>(i, j+offset) = chr.at<unsigned char>(i, j);
            }
        }

        cv::merge(new_img_channel, img);
        this->cpu_img = img;
        this->curr_dim.second--;
    }
}

cv::Mat SeamCarver::getOrigImage() const
{
    return this->orig_img;
}

cv::Mat SeamCarver::getImage() const
{
    return this->cpu_img;
}
cv::Mat SeamCarver::getCPUCarvedImage() const
{
    return this->cpu_img;
}
cv::Mat SeamCarver::getGPUCarvedImage() const
{
    return this->gpu_img;
}
std::pair<int, int> SeamCarver::getOrigDim() const
{
    return this->orig_dim;
}
std::pair<int, int> SeamCarver::getCarvedDim() const
{
    return this->curr_dim;
}

const bool SeamCarver::getIfReduce() const {
    return this->reduce;
}

void SeamCarver::setOptimalSeam(std::vector<unsigned int> &optimal_seam)
{
    this->optimal_seam = optimal_seam;
}

double SeamCarver::getExecutionTime() const
{
    return this->exec_time;
}

void SeamCarver::setExecutionTime(double &time)
{
    this->exec_time += time;
}
