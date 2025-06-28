#include "lista.hpp"
#include <iostream>
using namespace std;

int main() {
    list<double> ListaDouble = {10, 20, 30, 40, 50};

    cout << *ListaDouble.head;

    return 0;
}