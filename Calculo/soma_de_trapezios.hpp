#include "fx.hpp"
using namespace std;

double soma_de_trapezios(fx& func, double a, double b, size_t intervalos = 100000000) {
    const double base = (b - a) / intervalos;

    double soma = 0.5 * (func(a) + func(b));  // metade das pontas

    for (size_t i = 1; i < intervalos; ++i) {
        soma += func((a+(i*base)));
    }

    return soma * base;
}