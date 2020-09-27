Copy OpenCV Image:
```cpp
__global__ void copyImage(uint_8 *img, const int img_step, uint_8 *res, const int res_step)
{
    int c_col = blockIdx.x * blockDim.x + threadIdx.x;
    int c_row = blockIdx.y * blockDim.y + threadIdx.y;
    int gid = img_step * c_row + c_col * 3;
    int gid_res = res_step * c_row + c_col * 3;

    res[gid_res] = img[gid];
    res[gid_res + 1] = img[gid + 1];
    res[gid_res + 2] = img[gid + 2];
}
```

Convert image to gray scale:
```cpp
__global__ void convertToGrayscale(uint_8 *img, const int img_step, uint_8 *res, const int res_step)
{
    int c_col = blockIdx.x * blockDim.x + threadIdx.x;
    int c_row = blockIdx.y * blockDim.y + threadIdx.y;
    // NOTE: img_step != gridDim.x * blockDim.x * 3 because the step size is 
    // is determined by OpenCV. This is probably due to OpenCV using stided 
    // storage.
    int gid = img_step * c_row + c_col * 3;
    int gid_res = res_step * c_row + c_col;

    double px = (img[gid] + img[gid+1] + img[gid+2]) / 3.0;
    res[gid_res] = (uint_8) px;
}
```