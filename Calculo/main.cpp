#include <iostream>
#include "integral.hpp"
#include <cmath>
using namespace std;


int main() {
    string func = "cos(x)";
    double a = 0;
    double b = M_PI;
    cout << "Integral: " << soma_de_trapezios(func, a, b) << endl;

    return 0;
}

