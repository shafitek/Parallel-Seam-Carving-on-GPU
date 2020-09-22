#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

int test(int argc, char const *argv[])
{
    std::cout << "Parallel Seam Carving on the GPU!" << std::endl;

    if(argc < 1) {
        std::cout << "usage: SeamCarving <Image_Path>" << std::endl;
        return -1;
    }
    std::cout << "ARG COUNT: " << argc << std::endl;
    for(int i = 1; i < argc; i++)
        std::cout << argv[i] << std::endl;

    cv::Mat image;
    image = cv::imread(argv[1], -1);

    if( !image.data ) {
        printf("Invalid image.\n");
        return -1;
    }

    // std::cout << image.rows << " " << image.cols << std::endl;
    // cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    // cv::imshow("Display Image", image);

    // cv::waitKey(0);

    // get part of the image matrix
    cv::Mat A(image, cv::Rect(10, 10, 3, 5));
    cv::Mat B(image, cv::Rect(50, 50, 3, 5));
    // cv::Mat A = image(cv::Range::all(), cv::Range( (int)image.cols / 2, image.cols));

    // Equivalent to A[0:2, 2:4] *= 2 in numpy
    // A(cv::Range(0, 2), cv::Range(2, 4)) *= 2;

    std::cout << "A=\n"<< A << std::endl;
    std::cout << "B=\n" << B << std::endl;
    // cv::Mat B;
    // B = A.clone();
    cv::flip(A(cv::Range(0, 1), cv::Range::all()), A(cv::Range(0, 1), cv::Range::all()), 1);
    
    std::cout << A.row(2) << std::endl;
    // std::cout << B << std::endl;

    cv::Point3i pnt(2, 1, 1);
    std::cout << (int)A.row(3).at<uchar>(2) << std::endl;

    cv::Scalar Amean, AstdDev;
    cv::meanStdDev(A, Amean, AstdDev);
    std::cout << "MEAN =" << Amean << std::endl;

    cv::Mat C = (A >= 160);
    std::cout << C << std::endl;
    cv::bitwise_not(C, C);
    std::cout << C << std::endl;

    // double amin=0, amax=0;
    // cv::Point aminpoint, amaxpoint;
    // cv::minMaxLoc(C, &amin, &amax, NULL, NULL);
    // std::cout << amin << "|" << amax << std::endl;

    // std::cout << A.rows << " " << A.cols << std::endl;
    // cv::namedWindow("Display A", cv::WINDOW_AUTOSIZE);
    // cv::imshow("Display A", image);
    // cv::waitKey(0);

    cv::Mat matrix(5, 4, CV_8UC1);
    cv::randu(matrix, 0, 256);

    cv::Mat matrix2(5, 4, CV_8UC1);
    cv::randu(matrix2, 0, 256);

    cv::Mat mask(5, 4, CV_8UC1);
    cv::randu(mask, 0, 2);

    mask.setTo(255, mask>0);

    // matrix.setTo(9, mask);

    std::cout << "Rand. Matrix=\n" << matrix << std::endl << std::endl;
    std::cout << "Rand. Matrix2=\n" << matrix2 << std::endl << std::endl;
    std::cout << "Mask=\n" << mask << std::endl << std::endl;

    matrix.copyTo(matrix2, mask);
    std::cout << "Rand. Matrix2=\n" << matrix2 << std::endl << std::endl;

    std::cout << (unsigned int) matrix.at<uchar>(3, 2) << std::endl;
    // std::cout << matrix(cv::Range::all()) << std::endl;

    printf("\n\n\n");

    return 0;
}
