## OpenCV Cheatsheet

Basic structures: https://docs.opencv.org/2.4.2/modules/core/doc/basic_structures.html

Core matrix operations: https://docs.opencv.org/master/d0/d9c/core_2include_2opencv2_2core_8hpp.html

`Mat` is a general matrix container which can be used to store images: https://docs.opencv.org/master/d6/d6d/tutorial_mat_the_basic_image_container.html

A `Mat` object has a header and the matrix data. The data is stored in row major order. To perform deep copies of the `Mat` object, must use `Mat B = A.clone()`. For more:
https://docs.opencv.org/master/d3/d63/classcv_1_1Mat.html

Array operations: https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html, https://docs.opencv.org/master/d2/de8/group__core__array.html

---

Create a 3x4 image (3 channels, unsigned char) matrix: 
`Mat A(3, 4, CV_8UC3, Scalar(255, 255, 255))`

Create a 3D point: `Point3f P3f(2, 6, 7)` 

Create a matrix from vector of points:
```cpp
std::vector<Point3f> vec;
...
Mat pointMat = Mat(vec). // convert vector to Mat, O(1) operation
                  reshape(1); // make Nx3 1-channel matrix out of Nx1 3-channel.
                              // Also, an O(1) operation
```

Print the matrix: `std::cout << A << std::endl`

Print matrix in Python format: `std::cout << format(A, Formatter::FMT_PYTHON) << std::endl`

Number of rows, cols: `A.rows`, `A.cols`

Transpose a matrix: `A.t()`

Submatrix using Range: `A(Range(i, j), Range(k,l))`. Equiv. to `A[i:j, k:l]` in Numpy. Just like " : " in Matlab or " ... " in Python - slicing.

Submatrix using clipping: `Mat N(A, Rect(x, y, w, h))`

Split, Merge matrix channels:
```cpp
Mat img(5,5,CV_64FC3);
Mat ch1, ch2, ch3;
// "channels" is a vector of 3 Mat arrays:
vector<Mat> channels(3);

///////////////// SPLIT ///////////////
split(img, channels);
// (BGR order in OpenCV)
ch1 = channels[0];
ch2 = channels[1];
ch3 = channels[2];

///////////////// MERGE ///////////////

merge(channels, 3, img)
```

Zeros, Ones, Diagonals:
```cpp
Mat E = Mat::eye(4, 4, CV_64F) * SOME_DIAG_VAL;
cout << "E = " << endl << " " << E << endl << endl;
Mat O = Mat::ones(2, 2, CV_32F);
cout << "O = " << endl << " " << O << endl << endl;
Mat Z = Mat::zeros(3,3, CV_8UC1);
cout << "Z = " << endl << " " << Z << endl << endl;
```

Create a random vecotor/matrix (Uniform, Normal):
```cpp
Mat1d mat(2, 4); // Or: Mat mat(2, 4, CV_64FC1);
double low = -500.0;
double high = +500.0;
randu(mat, Scalar(low), Scalar(high));

Mat1d mat(2, 4); // Or: Mat mat(2, 4, CV_64FC1);
double mean = 0.0;
double stddev = 500.0 / 3.0; // 99.7% of values will be inside [-500, +500] interval
randn(mat, Scalar(mean), Scalar(stddev));
```

Flip/Reverse an sub-matrix in-place:
```cpp
cv::flip(A(cv::Range(0, 1), cv::Range(0, 3)), A(cv::Range(0, 1), cv::Range(0, 3)), 1);
```

Get row, col then clone:
```cpp
Mat RowOne = C.row(1);
Mat ColOneClone = C.col(1).clone(); // if you want to clone the vector
```

Indexing into the matrix:
```cpp
A.at<uchar>(2, 1) // gets element at second row, first column
A.row(i).col(j) // get [b, g, r] for the pixel at {i, j}
A.row(i).col(j).at<uchar>(1) // get `g` for the pixel at {i, j}
``` 

Obtain matrix main diagonal: `Mat d0 = A.diag(0)`

Sum a matrix:
```cpp
sum(A) // sum a matrix by channel. If matrix has 3 channels, then output [sum(b), sum(g), sum(r), 0]
sum(sum(A)) // to sum the entire matrox -> [sum(b)+sum(g)+sum(r), 0, 0, 0]
```

Max, Min, Mean, Std. Dev. of a array/matrix:
```cpp
mean(A) // mean for each channel

Scalar Amean, AstdDev; // Scalar type can be used for 4-element vectors. Mainly used to pass pixel values. 
meanStdDev(A, Amean, AstdDev)

Mat C = min(A, SOME_VAL) // C[i,j] = min(A[i, j], SOME_VAL)
Mat C = min(A, B) // C[i,j] = min(A[i, j], B[i, j])
Mat C = max(A, SOME_VAL) // C[i,j] = min(A[i, j], SOME_VAL)
Mat C = max(A, B) // C[i,j] = max(A[i, j], B[i, j])

double minA=0, maxA=0;
cv::Point minLoc, maxLoc;
minMaxLoc(A, &minA, &maxA, &minLoc, &maxLoc, OPTIONAL_MASK) // finds the minimum and maximum element values and their positions. ONLY works with single channel!
```

Matrix Comapre and Bitwise Operations, Matrix mask
```cpp
Mat C = A >= SOME_NUM // Creates a mask matrix, 0 if false, 255 if true
Mat C = A >= B // Matrix element to element comparison
// Alternatively,
Mat C;
compare(A, B, C, CMP_EQ) // CMP_EQ is an enum -> See cv::CmpTypes

bitwise_not(C, C, OPTIONAL_MASK) // flip elements of C -> 0 to 255 and 255 to 0
```

Using masks:
```cpp
A.setTo(SOME_VAL, mask); // set mask elements to some number
A.copyTo(B, mask); // copy over elements of A to the corresponding elements of B specified by the mask.
```

Matrix Concatenate:
```cpp
// VERTICAL
cv::Mat matArray[] = { cv::Mat(1, 4, CV_8UC1, cv::Scalar(1)),
                       cv::Mat(1, 4, CV_8UC1, cv::Scalar(2)),
                       cv::Mat(1, 4, CV_8UC1, cv::Scalar(3)),};
cv::Mat out;
cv::vconcat( matArray, 3, out );
//out:
//[1,   1,   1,   1;
// 2,   2,   2,   2;
// 3,   3,   3,   3]

// HORIZONTAL
cv::Mat matArray[] = { cv::Mat(4, 1, CV_8UC1, cv::Scalar(1)),
                       cv::Mat(4, 1, CV_8UC1, cv::Scalar(2)),
                       cv::Mat(4, 1, CV_8UC1, cv::Scalar(3)),};
cv::Mat out;
cv::hconcat( matArray, 3, out );
//out:
//[1, 2, 3;
// 1, 2, 3;
// 1, 2, 3;
// 1, 2, 3]
```

Read an image: 
```cpp
cv::Mat image;
image = cv::imread(argv[1], -1);
```

Display an image:
```cpp
cv::namedWindow("Display A", cv::WINDOW_AUTOSIZE);
cv::imshow("Display A", A);
cv::waitKey(0);
```