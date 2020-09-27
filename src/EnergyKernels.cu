#include <stdint.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "EnergyKernels.cuh"

#define BLK_WIDTH 32
#define BLK_HEIGHT 32
#define BLK_CHANNEL 3

typedef unsigned char uint_8;

#define gpuErrchk(ans)                        \
    {                                         \
        gpuAssert((ans), __FILE__, __LINE__); \
    }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort)
            exit(code);
    }
}

__global__ void calcEnergyImg(double *img, double *energy_img);
__global__ void calcEnergyMap(double *img, double *energy_map);

__global__ void convertBGR2GRAYGPU(uint_8 *img, const int img_step, uint_8 *res, const int res_step, int width)
{
    int c_col = blockIdx.x * blockDim.x + threadIdx.x;

    if(c_col >= width) return;

    int c_row = blockIdx.y * blockDim.y + threadIdx.y;
    int gid = img_step * c_row + c_col * 3;
    int gid_res = res_step * c_row + c_col;
    
    double px = (img[gid] + img[gid+1] + img[gid+2]) / 3.0;
    res[gid_res] = (uint_8) px;
}

__global__ void computeEnergyImageGPU(uint_8 *img, const int img_step, uint_8 *res, const int res_step, int width, int height)
{
    int c_col = blockIdx.x * blockDim.x + threadIdx.x;

    if(c_col >= width) return;

    int c_row = blockIdx.y * blockDim.y + threadIdx.y;
    int gid = img_step * c_row + c_col;
    int gid_r = res_step * c_row + c_col;

    uint_8 xg = c_col != width;
    uint_8 yg = c_row != height;

    int diffx, diffy, diffxy, tmpPx;
    diffx = xg ? abs(img[gid] - img[gid+1]) : 0;
    diffy = yg ? abs(img[gid] - img[gid + img_step]) : 0;
    diffxy = xg ? abs(img[gid] - img[gid + img_step + 1]) : 0;
    tmpPx = min(255, diffx + diffy + diffxy);

    res[gid_r] = (uint_8)tmpPx;
}

__global__ void copyFirstRow(uint_8 *img, double *res, int width)
{
    int gid = blockIdx.x * blockDim.x + threadIdx.x;

    if(gid >= width) return;

    res[gid] = img[gid];
}

__global__ void computeEnergyMapGPU(uint_8 *img, const int img_step, double *res, const int res_step, int width, int row, int block_num)
{
    unsigned int col = block_num * blockDim.x + threadIdx.x;

    if(col >= width) return;

    int gid = row * img_step + col;

    // https://stackoverflow.com/questions/44174092/how-to-multiply-two-opencv-matrices-in-a-kernel-function-in-cuda
    // Access the double array like ((double *)((unsigned char *)res + row * res_step))[col].

    double left, middle, right;
    left = col % width != 0 ? ((double *)((unsigned char *)res + (row - 1) * res_step))[col - 1] : INT_MAX;
    middle = ((double *)((unsigned char *)res + (row - 1) * res_step))[col];
    right = (width - 1) != col ? ((double *)((unsigned char *)res + (row - 1) * res_step))[col + 1] : INT_MAX;

    ((double *)((unsigned char *)res + row * res_step))[col] 
            = img[gid] + min(middle, min(left, right));
}

void computeEnergyMapGPU(const cv::Mat &c_imgi, cv::Mat &f_ener_img, cv::Mat &f_ener_map, double &time)
{
    uint8_t *img_ptr, *gry_ptr;
    cv::Mat c_img = c_imgi;
    f_ener_img.create(c_img.rows, c_img.cols, CV_8U);
    f_ener_map.create(c_img.rows, c_img.cols, CV_64F);
    cv::Mat &c_ener_img = f_ener_img;
    cv::Mat &c_ener_map = f_ener_map;

    auto start = std::chrono::high_resolution_clock::now();
    cv::cuda::GpuMat g_img;
    g_img.upload(c_img);

    dim3 block(BLK_WIDTH, BLK_HEIGHT);
    dim3 grid((g_img.cols + BLK_WIDTH - 1) / BLK_WIDTH, (c_img.rows + BLK_WIDTH - 1) / BLK_HEIGHT);

    cudaMalloc((void **)&img_ptr, g_img.rows * g_img.step);
    cudaMemcpyAsync(img_ptr, g_img.ptr<uint8_t>(), g_img.rows * g_img.step, cudaMemcpyDeviceToDevice);

    // cv::cuda::GpuMat g_gry_img(c_img.rows, c_img.cols, c_ener_img.type(), img_ptr, c_ener_img.step);

    // convertBGR2GRAYGPU<<<grid, block>>>(
    //     (uint_8 *)g_img.data, g_img.step,
    //     (uint_8 *)g_gry_img.data, g_gry_img.step,
    //     c_img.cols);

    // gry_ptr = g_gry_img.ptr<uint8_t>();
    cv::cuda::GpuMat g_ener_img(c_img.rows, c_img.cols, c_ener_img.type(), img_ptr, c_ener_img.step);

    computeEnergyImageGPU<<<grid, block>>>(
        (uint_8 *)g_img.data, g_img.step,
        (uint_8 *)g_ener_img.data, g_ener_img.step,
        c_img.cols, c_img.rows);

    cv::cuda::GpuMat g_ener_map(c_img.rows, c_img.cols, CV_64F);

    copyFirstRow<<<(c_img.cols  + BLK_WIDTH - 1) / BLK_WIDTH, BLK_WIDTH>>>(
        (uint_8 *)g_ener_img.data,
        (double *)g_ener_map.data,
        c_img.cols);

    cudaDeviceSynchronize();

    const size_t num_streams = (c_img.cols + BLK_WIDTH - 1) / BLK_WIDTH;

    for(int row = 1; row < c_img.rows; row++) {
        cudaStream_t streams[num_streams];
        for (int st = 0; st < num_streams; st++)
        {
            cudaStreamCreate(&streams[st]);
            computeEnergyMapGPU<<<1, BLK_WIDTH, 0, streams[st]>>>(
                (uint_8 *)g_ener_img.data, g_ener_img.step,
                (double *)g_ener_map.data, g_ener_map.step,
                c_img.cols, row, st);
        }
        for (int sd = 0; sd < num_streams; sd++)
            cudaStreamDestroy(streams[sd]);
    }

    g_ener_img.download(c_ener_img);
    g_ener_map.download(c_ener_map);
    // gpuErrchk(cudaPeekAtLastError());

    auto end = std::chrono::high_resolution_clock::now();

    time = (double)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    cudaFree(img_ptr);
    cudaFree(gry_ptr);

    cudaDeviceReset();
}