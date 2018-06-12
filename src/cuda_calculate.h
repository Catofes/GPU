#ifndef CUDA_CALCULADE
#define CUDA_CALCULADE

/* Use GPU to calculate
*      F(x) = \int_{x_{min}-3\sigma({x_{min}})}^{x_{max}+3\sigma({x_{max}})} f(t) *
*          \frac{1}{\sigma (t)} \exp{-\frac{(t-x)^2}{2*\sigma^2(t)}}
*/
double sub_cuda_normal_calculate(int bins,
                                 double x_min,
                                 double x_max,
                                 double x,
                                 double mean_1,
                                 double mean_2,
                                 double width_1,
                                 double width_2,
                                 double a);

#endif
