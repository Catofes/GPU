#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/sequence.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/replace.h>
#include <thrust/functional.h>
#include <iostream>
#include <thrust/complex.h>
#include "cuda_calculate.h"
#include <chrono>
#include <cuda.h>

#define GPUDEBUG

struct sub_f
{
    const double max;
    const double min;
    const double mean;
    const double width;


    sub_f(double _max, double _min, double _mean, double _width) : max(_max), min(_min), mean(_mean), width(_width)
    {}

    __device__
    double operator()(const double &x) const
    {
        double _x = x;
        if (x > max) {
        //    _x = 2 * max - x;
        }
        if (x < min) {
        //    _x = 2 * min - x;
        }
        double w = (width > 0) ? width : -width;
        return 1. / ((_x - mean) * (_x - mean) + 0.25 * w * w);
    }
};

struct sub_sigma
{
    const double sigma;

    sub_sigma(double _sigma) : sigma(_sigma)
    {}

    __device__
    double operator()(const double &x) const
    {
        return sigma;
    }
};

struct sub_gauss
{
    const double t;

    sub_gauss(double _t) : t(_t)
    {}

    __device__
    double operator()(const double &x, const double &sigma) const
    {
//        thrust::complex<double> v(-0.5 / (sigma * sigma) * (x - t) * (x - t), 0);
//        return thrust::exp(v).real() / sigma;
        return exp(-0.5 / (sigma * sigma) * (x - t) * (x - t)) / sigma;
    }
};


thrust::device_vector<double> *d_t = nullptr;
thrust::device_vector<double> *d_sigma = nullptr;
thrust::device_vector<double> *d_x = nullptr;
thrust::device_vector<double> *d_w = nullptr;
//std::vector<double> *h_result = nullptr;

std::vector<double>
sub_cuda_normal_calculate(int bins, double min, double max, double x, double mean, double width, double f_min,
                          double f_max)
{
    std::vector<double> result;
    result.push_back(0);
#ifdef GPUDEBUG
    std::chrono::system_clock::time_point start, finish;
#endif

    if (d_t == nullptr) {
        d_t = new thrust::device_vector<double>(bins);
    }
    if (d_sigma == nullptr) {
        d_sigma = new thrust::device_vector<double>(bins);
    }
//    if (h_result == nullptr) {
//        h_result = new std::vector<double>(bins);
//    }
#ifdef GPUDEBUG
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::sequence((*d_t).begin(), (*d_t).end(), min + 0.5 * (max - min) / bins, (max - min) / bins);

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), sub_sigma(0.5));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_sigma).begin(), sub_gauss(x));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_t).begin(), sub_f(f_max, f_min, mean, width));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_t).begin(), thrust::multiplies<double>());

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    result[0] = thrust::reduce((*d_t).begin(), (*d_t).end(), double(0), thrust::plus<double>());

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif
//
//    thrust::copy((*d_t).begin(), (*d_t).end(), (*h_result).begin());
//
//#ifdef GPUDEBUG
//    finish = std::chrono::high_resolution_clock::now();
//    std::cout << "s7: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
//    start = std::chrono::high_resolution_clock::now();
//#endif
//
//    double r;
//    for (int i = 0; i < bins; i++) {
//        r += (*h_result)[i];
//    }
//
//#ifdef GPUDEBUG
//    finish = std::chrono::high_resolution_clock::now();
//    std::cout << "s8: " << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << std::endl;
//    start = std::chrono::high_resolution_clock::now();
//#endif

    return result;
}

void sub_cuda_gaus_prepare(std::vector<double> &x, std::vector<double> &w, int bins)
{
    if (d_x != nullptr) {
        delete d_x;
    }
    if (d_w != nullptr) {
        delete d_w;
    }
    d_x = new thrust::device_vector<double>(bins);
    d_w = new thrust::device_vector<double>(bins);
    thrust::copy(x.begin(), x.end(), (*d_x).begin());
    thrust::copy(w.begin(), w.end(), (*d_w).begin());
}

std::vector<double>
sub_cuda_gaus_calculate(int bins, double min, double max, double x, double mean, double width, double f_min,
                        double f_max)
{
    std::vector<double> result;
    result.push_back(0);
#ifdef GPUDEBUG
    std::chrono::system_clock::time_point start, finish;
#endif

    if (d_t == nullptr) {
        d_t = new thrust::device_vector<double>(bins);
    }
    if (d_sigma == nullptr) {
        d_sigma = new thrust::device_vector<double>(bins);
    }
#ifdef GPUDEBUG
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::copy((*d_x).begin(), (*d_x).end(), (*d_t).begin());

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), sub_sigma(0.5));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_sigma).begin(), sub_gauss(x));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_t).begin(), sub_f(f_max, f_min, mean, width));

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_t).begin(), thrust::multiplies<double>());
    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_w).begin(), (*d_t).begin(), thrust::multiplies<double>());

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    result[0] = thrust::reduce((*d_t).begin(), (*d_t).end(), double(0), thrust::plus<double>());

#ifdef GPUDEBUG
    finish = std::chrono::high_resolution_clock::now();
    result.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    start = std::chrono::high_resolution_clock::now();
#endif

    return result;
}