#ifndef CALCULUS_LIBRARY
#define CALCULUS_LIBRARY

#include <math.h>

double derivative(double (*in_func)(double), double x)
{
    double out;
    double h = 0.000001;
    out = (in_func(x + h) - in_func(x)) / h;
    return out;
}

double integral(double (*in_func)(double), double xi, double xf)
{
    unsigned int step_count = 5000;
    double out = 0.0;
    double delta = xf - xi;
    double step_size = delta / double(step_count);
    for (unsigned int n = 0; n < step_count; n++)
    {
        out += (in_func(xi + double(n) * step_size) + in_func(xi + double(n + 1) * step_size)) * step_size / 2.0;
    }
    return out;
}

#endif
