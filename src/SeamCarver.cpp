#include "SeamCarver.h"

SeamCarver::SeamCarver(const char *img_path, const bool &reduce)
{
    this->curr_img = cv::imread(img_path, cv::IMREAD_COLOR);
    this->orig_img = this->curr_img.clone();
    cv::cvtColor(this->orig_img, this->gray_orig_image, cv::COLOR_BGR2GRAY);
    this->reduce = reduce;

    this->orig_dim = std::make_pair(this->orig_img.rows, this->orig_img.cols);
    this->curr_dim = std::make_pair(this->orig_img.rows, this->orig_img.cols);
}

void SeamCarver::carveImage(const unsigned int &resize_factor)
{
    for(int m = 0; m < resize_factor; m++) {
        computeEnergyImage();
        computeEnergyMap();
        computeOptimalSeam();
        
        cv::Mat img = this->curr_img;

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
        this->curr_img = img;
        cv::cvtColor(this->curr_img, this->gray_orig_image, cv::COLOR_BGR2GRAY);
        this->curr_dim.second--;
    }
}

void SeamCarver::computeOptimalSeam()
{
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat &emap = this->energy_map;
    int height = emap.rows;
    int width = emap.cols;

    cv::Mat last_row = emap.row(height - 1);
    double min_v, max_v;
    cv::Point min_of_col, max_of_col;

    cv::minMaxLoc(last_row, &min_v, &max_v, &min_of_col, &max_of_col);

    unsigned int left, middle, right, curr_col;
    curr_col = min_of_col.x;
    std::vector<unsigned int> curr_seam(height, 0);

    for (int i = height - 2; i >= 0; i--)
    {
        left = curr_col - 1;
        middle = curr_col;
        right = curr_col + 1;
        if (left < 0)
        {
            if (emap.at<double>(i, middle) > emap.at<double>(i, right))
                curr_col = right;
            else
                curr_col = middle;

            curr_seam[i] = curr_col;
            continue;
        }
        if (right > width - 1)
        {
            if (emap.at<double>(i, left) > emap.at<double>(i, middle))
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

cv::Mat SeamCarver::getOrigImage() const
{
    return this->orig_img;
}

cv::Mat SeamCarver::getImage() const
{
    return this->gray_orig_image;
}
cv::Mat SeamCarver::getCarvedImage() const
{
    return this->curr_img;
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
