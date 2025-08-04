#include <iostream>
#include "soma_de_trapezios.hpp"
#include "fx.hpp"
#include <cmath>
using namespace std;


int main() {
    fx func("tan(x)");
    double a = 0;
    double b = 10000000000;
    cout << "Integral: " << soma_de_trapezios(func, a, b) << endl;

    return 0;
}

