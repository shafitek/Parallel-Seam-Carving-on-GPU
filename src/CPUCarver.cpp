#include "CPUCarver.h"

CPUCarver::CPUCarver(const char *img_path, const bool &reduce)
    : SeamCarver(img_path, reduce){};

void CPUCarver::computeEnergyImage() {
    auto start = std::chrono::high_resolution_clock::now();
    const cv::Mat img = getImage();
    cv::Mat efm(img.rows, img.cols, img.type());

    for(int i = 0; i < efm.rows; i++) {
        for(int j = 0; j < efm.cols; j++) {
            efm.at<unsigned char>(i, j) = std::abs(img.at<unsigned char>(i, j) - img.at<unsigned char>(i, j + 1)) +
                                          std::abs(img.at<unsigned char>(i, j) - img.at<unsigned char>(i + 1, j)) +
                                          std::abs(img.at<unsigned char>(i, j) - img.at<unsigned char>(i + 1, j + 1));
        }
    }
    
    efm.convertTo(efm, CV_64F);
    this->energy_img = efm;
    auto end = std::chrono::high_resolution_clock::now();
    double diff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    setExecutionTime(diff);
}

void CPUCarver::computeEnergyMap()
{
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat &eimg = this->energy_img;
    cv::Mat emap = cv::Mat::zeros(eimg.rows, eimg.cols, CV_64F);

    eimg.row(0).copyTo(emap.row(0));

    for (int i = 1; i < emap.rows; i++)
    {
        for (int j = 1; j < emap.cols - 1; j++)
        {
            emap.at<double>(i, j) = eimg.at<double>(i, j) +
                                   std::min(emap.at<double>(i - 1, j - 1),
                                            std::min(emap.at<double>(i - 1, j),
                                                     emap.at<double>(i - 1, j + 1)));
        }

        emap.at<double>(i, 0) = eimg.at<double>(i, 0) +
                               std::min(emap.at<double>(i - 1, 0),
                                        emap.at<double>(i - 1, 1));

        emap.at<double>(i, eimg.cols - 1) =
            eimg.at<double>(i, eimg.cols - 1) +
            std::min(emap.at<double>(i - 1, emap.cols - 2),
                     emap.at<double>(i - 1, emap.cols - 1));
    }

    this->energy_map = emap;
    auto end = std::chrono::high_resolution_clock::now();
    double diff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    setExecutionTime(diff);
}
