/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

// Your description goes here... 

#include <complex>
#include <RooMath.h>
#include <chrono>
#include "Riostream.h"

#include "RooMyPdf.h"
#include "cuda_calculate.h"

ClassImp(RooMyPdf)

RooMyPdf::RooMyPdf(const char *name, const char *title,
                   RooAbsReal &_x, RooAbsReal &_mean, RooAbsReal &_width, int _bins, int _method, DebugTimeInfo *_debug)
        :
        RooAbsPdf(name, title),
        x("x", "x", this, _x),
        mean("mean", "mean", this, _mean),
        width("width", "Breit-Wigner Width", this, _width),
        method(_method),
        bins(_bins),
        gaus_x(_bins),
        gaus_w(_bins),
        debug(_debug)
{
    string a = "time_";
    a += string(name);
    if (_method == 0 || _method == 2) {
        temp = new fastgl::QuadPair[bins];
        for (int i = 0; i < bins; i++) {
            temp[i] = fastgl::GLPair(bins, i + 1);
        }
    }
}


RooMyPdf::RooMyPdf(const RooMyPdf &other, const char *name) :
        RooAbsPdf(other, name),
        x("x", this, other.x),
        mean("mean", this, other.mean),
        width("width", this, other.width),
        method(other.method),
        bins(other.bins),
        gaus_x(other.bins),
        gaus_w(other.bins),
        debug(other.debug)
{
    if (other.method == 0 || other.method == 2) {
        temp = new fastgl::QuadPair[bins];
        for (int i = 0; i < bins; i++) {
            temp[i] = other.temp[i];
        }
    }
    for (int i = 0; i < bins; i++) {
        gaus_x[i] = other.gaus_x[i];
        gaus_w[i] = other.gaus_w[i];
    }
}

// F(t) = \frac{1}{(t-mean)^2+0.25\times width^2}
Double_t RooMyPdf::sub_f(Double_t t) const
{
    Double_t upper = x.max();
    Double_t lower = x.min();
//    if (t > upper) {
//        //t = 2 * upper - t;
//    }
//    if (t < lower) {
//        //t = 2 * lower - t;
//    }

    Double_t w = (width > 0) ? width : -width;
    Double_t arg = t - mean;
    return (1. / (arg * arg + 0.25 * w * w));
}

Double_t RooMyPdf::sub_sigma(Double_t t) const
{
    return 3;
    //return sqrt(15)*3/sqrt(abs(t));
}

// F(t) = f(t) * \frac{1}{\sigma (t)} \exp{-\frac{(t-x)^2}{2*\sigma^2(t)}}
Double_t RooMyPdf::sub_evaluate(Double_t t) const
{
    Double_t s = (sub_sigma(t) > 0) ? sub_sigma(t) : -sub_sigma(t);
    Double_t arg = t - x;
    Double_t coef = -0.5 / (s * s);
    double_t result = sub_f(t) * exp(coef * arg * arg) * 1 / s;
    return result;
}

Double_t RooMyPdf::cuda_normal_evaluate() const
{
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    auto result = sub_cuda_normal_calculate(bins, lower, upper, x, mean, width, x.min(), x.max());
    result[0] = result[0] * (upper - lower) / bins;
    if (debug == nullptr) {
        return result[0];
    }
    for (int i = 1; i < result.size(); i++) {
        auto key = "s" + to_string(i);
        debug->AddValues(key, result[i]);
    }
    return result[0];
}

Double_t RooMyPdf::cuda_normal_evaluate_tuned() const
{
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    auto result = sub_cuda_normal_calculate_tuned(bins, lower, upper, x, mean, width, x.min(), x.max());
    result[0] = result[0] * (upper - lower) / bins;
    if (debug == nullptr) {
        return result[0];
    }
    for (int i = 1; i < result.size(); i++) {
        auto key = "s" + to_string(i);
        debug->AddValues(key, result[i]);
    }
    return result[0];
}

void RooMyPdf::cuda_gaus_prepare()
{
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    for (int i = 0; i < bins; i++) {
        auto p = temp[i];
        gaus_x[i] = (upper - lower) / 2. * p.x() + (upper + lower) / 2.;
        gaus_w[i] = p.weight * (upper - lower) / 2.;
    }
    sub_cuda_gaus_prepare(gaus_x, gaus_w, bins);
}

Double_t RooMyPdf::cuda_gaus_evaluate() const
{
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    auto result = sub_cuda_gaus_calculate(bins, lower, upper, x, mean, width, x.min(), x.max());
    if (debug == nullptr) {
        return result[0];
    }
    for (int i = 1; i < result.size(); i++) {
        auto key = "s" + to_string(i);
        debug->AddValues(key, result[i]);
    }
    return result[0];
}

Double_t RooMyPdf::gaus_evaluate() const
{
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    Double_t result = 0;
    for (int i = 0; i < bins; i++) {
        auto p = temp[i];
        result += p.weight * sub_evaluate((upper - lower) / 2. * p.x() + (upper + lower) / 2.);
    }
    result = (upper - lower) / 2. * result;
    return result;
}

Double_t RooMyPdf::normal_evaluate() const
{
    int cut = bins;
    Double_t upper = x.max() + 3 * sub_sigma(x.max());
    Double_t lower = x.min() - 3 * sub_sigma(x.min());
    double step = (upper - lower) / cut;
    double result = 0;
    for (int i = 0; i < cut; i++) {
        double point = lower + (i + 0.5) * step;
        result += sub_evaluate(point) * step;
    }
    return result;
}

Double_t RooMyPdf::evaluate() const
{
    if (debug != nullptr) {
        debug->Clean();
        debug->AddValues("type", double(method));
    }
    auto start = std::chrono::high_resolution_clock::now();
    Double_t result = 0;
    switch (method) {
        case 0:
            result = gaus_evaluate();
            break;
        case 1:
            result = normal_evaluate();
            break;
        case 2:
            result = cuda_gaus_evaluate();
            break;
        case 3:
            result = cuda_normal_evaluate();
            break;
        case 4:
            result = cuda_normal_evaluate_tuned();
            break;
    }
    auto finish = std::chrono::high_resolution_clock::now();
    if (debug != nullptr) {
        debug->AddValues("total", std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
        debug->Save();
    }
    return result;
}

