#ifndef SHAFITEK_UTILS_H
#define SHAFITEK_UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

bool compareChar(char &, char &);
bool istrEquals(std::string &, std::string &);
int ifCPUGPUBoth(const char *);
void viewBeforeAfter(cv::Mat &, cv::Mat &);
void eraseTextFromTerminal(int);

#endif // SHAFITEK_UTILS_H