#include "fx.hpp"
using namespace std;

double soma_de_trapezios(const std::string& func_string, double a, double b) {
    size_t intervalos = 10000000;  // número de subintervalos
    double base = (b - a) / intervalos;

    fx func(func_string);

    double soma = 0.5 * (func(a) + func(b));  // metade das pontas

    for (size_t i = 1; i < intervalos; ++i) {
        soma += func((a+(i*base)));
    }

    return soma * base;
}