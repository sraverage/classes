#include <iostream>
#include "ponto.hpp"

int main() {
    ponto<2> A(0, 0);
    ponto<2> B(3, 5);

    std::cout << A.get_distance(B);




    return 0;
}