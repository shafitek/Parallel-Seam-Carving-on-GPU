#include "utils.h"

bool compareChar(char &c1, char &c2) {
    if (c1 == c2)
        return true;
    else if (std::toupper(c1) == std::toupper(c2))
        return true;
    return false;
}

bool istrEquals(std::string &s1, std::string &s2) {
    return (
        s1.size() == s2.size() &&
        std::equal(s1.begin(), s1.end(), s2.begin(), &compareChar)
    );
}

int ifCPUGPUBoth(const char *str) {
    std::string sstr = str;
    std::string cpu = "CPU";
    std::string gpu = "GPU";
    if (istrEquals(cpu, sstr))
        return 0;
    else if (istrEquals(gpu, sstr))
        return 1;
    return 2;
}

void viewBeforeAfter(cv::Mat &beforeImg, cv::Mat &afterImg){
    std::string title = "title";
    cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
    cv::imshow("Before", beforeImg);
    cv::imshow(title, afterImg);
    cv::waitKey(0);
}

void eraseTextFromTerminal(int count) {
    for(int i = 0; i < count; i++)
        std::cout << "\b" << std::flush;
}

void printCVMatrix(cv::Mat mat) {
    std::cout << cv::format(mat, cv::Formatter::FMT_MATLAB) << std::endl;
}