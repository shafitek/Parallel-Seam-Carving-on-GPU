#include "CPUCarver.h"

CPUCarver::CPUCarver(const char *img_path, const bool &reduce)
    : SeamCarver(img_path, reduce){};

void CPUCarver::computeEnergyMap()
{
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat x_out, y_out;
    cv::Mat x_filter = cv::Mat({-1, 1}).t();
    cv::Mat y_filter = cv::Mat({1, -1});

    cv::Mat grayMat;
    cv::cvtColor(getImage(), grayMat, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(grayMat, grayMat, cv::Size(3, 3), 0 , 0);

    cv::filter2D(grayMat, x_out, CV_16S, x_filter);
    cv::filter2D(grayMat, y_out, CV_16S, y_filter);

    cv::convertScaleAbs(x_out, x_out);
    cv::convertScaleAbs(y_out, y_out);

    cv::addWeighted(x_out, 0.5, y_out, 0.5, 0, this->energy_img);
    this->energy_img.convertTo(this->energy_img, CV_64F, 1.0 / 255.0);

    cv::Mat &eimg = this->energy_img;
    cv::Mat emap = cv::Mat::zeros(grayMat.rows, grayMat.cols, CV_64F);

    eimg.row(0).copyTo(emap.row(0));

    for (int i = 1; i < grayMat.rows; i++)
    {
        for (int j = 1; j < grayMat.cols - 1; j++)
        {
            emap.at<double>(i, j) = eimg.at<double>(i, j) +
                                   std::min(emap.at<double>(i - 1, j - 1),
                                            std::min(emap.at<double>(i - 1, j),
                                                     emap.at<double>(i - 1, j + 1)));
        }

        emap.at<double>(i, 0) = eimg.at<double>(i, 0) +
                               std::min(emap.at<double>(i - 1, 0),
                                        emap.at<double>(i - 1, 1));

        emap.at<double>(i, grayMat.cols - 1) =
            eimg.at<double>(i, grayMat.cols - 1) +
            std::min(emap.at<double>(i - 1, grayMat.cols - 2),
                     emap.at<double>(i - 1, grayMat.cols - 1));
    }

    this->energy_map = emap;
    auto end = std::chrono::high_resolution_clock::now();
    double diff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    setExecutionTime(diff);
}

void CPUCarver::computeOptimalSeam()
{
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat &emap = this->energy_map;
    int height = emap.rows;
    int width = emap.cols;

    cv::Mat last_row = emap.row(height-1);
    double min_v, max_v;
    cv::Point min_of_col, max_of_col;

    cv::minMaxLoc(last_row, &min_v, &max_v, &min_of_col, &max_of_col);

    unsigned int left, middle, right, curr_col;
    curr_col = min_of_col.x;
    std::vector<unsigned int> curr_seam(height, 0);

    for(int i = height-1; i >= 0; i--) {
        left = curr_col-1; middle = curr_col; right = curr_col+1;
        if (left < 0) {
            if (emap.at<double>(i, middle) > emap.at<double>(i, right))
                curr_col = right;
            else
                curr_col = middle;

            curr_seam[i] = curr_col;
            continue;
        }
        if (right > width-1)
        {
            if(emap.at<double>(i, left) > emap.at<double>(i, middle))
                curr_col = middle;
            else
                curr_col = left;

            curr_seam[i] = curr_col;
            continue;
        }
        std::vector<double> v = {emap.at<double>(i, left), 
                                emap.at<double>(i, middle), 
                                emap.at<double>(i, right)};

        int min_elem_idx = std::min_element(v.begin(), v.end()) - v.begin();
        if (min_elem_idx == 0)
            curr_col = left;
        else if (min_elem_idx == 2)
            curr_col = right;
        else
            curr_col = middle;
        
        curr_seam[i] = curr_col;
    }

    setOptimalSeam(curr_seam);
    auto end = std::chrono::high_resolution_clock::now();
    double diff = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    setExecutionTime(diff);
}
