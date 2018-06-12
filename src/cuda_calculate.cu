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
#include "thrust/system/cuda/detail/reduce.h"

struct sub_f
{
    const double mean_1;
    const double mean_2;
    const double width_1;
    const double width_2;
    const double a;

    //Construct function
    sub_f(double _mean_1, double _mean_2, double _width_1, double _width_2, double _a) :
            mean_1(_mean_1), mean_2(_mean_2), width_1(_width_1), width_2(_width_2), a(_a)
    {}

    //Calculate function
    __device__
    double operator()(const double &x) const
    {
        double w1 = (width_1 > 0) ? width_1 : -width_1;
        double w2 = (width_2 > 0) ? width_2 : -width_2;
        double arg1 = x - mean_1;
        double arg2 = x - mean_2;
        return (1. / (arg1 * arg1 + 0.25 * w1 * w1) + a / (arg2 * arg2 + 0.25 * w2 * w2));
    }
};

struct sub_sigma
{
    sub_sigma()
    {}

    __device__
    double operator()(const double &x) const
    {
        return 10 / sqrt(x);
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

double sub_cuda_normal_calculate(int bins,
                                 double x_min,
                                 double x_max,
                                 double x,
                                 double mean_1,
                                 double mean_2,
                                 double width_1,
                                 double width_2,
                                 double a)
{

    if (d_t == nullptr) {
        d_t = new thrust::device_vector<double>(bins);
    }
    if (d_sigma == nullptr) {
        d_sigma = new thrust::device_vector<double>(bins);
    }
    thrust::sequence((*d_t).begin(), (*d_t).end(), x_min + 0.5 * (x_max - x_min) / bins, (x_max - x_min) / bins);

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), sub_sigma());

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_sigma).begin(), sub_gauss(x));

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_t).begin(), sub_f(mean_1, mean_2, width_1, width_2, a));

    thrust::transform((*d_t).begin(), (*d_t).end(), (*d_sigma).begin(), (*d_t).begin(), thrust::multiplies<double>());

    return thrust::reduce((*d_t).begin(), (*d_t).end(), double(0), thrust::plus<double>());
}
