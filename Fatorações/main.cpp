#include <iostream>
#include "matriz.hpp"
#include "funcoesMatriz.hpp"
#include "fatoracaoQR.hpp"
#include "fatoracaoEspectral.hpp"
#include "fatoracaoSVD.hpp"
#include "string"
#include "sstream"
using namespace std;

void p(string s) {
    cout << s << "\n";
}

int main() {
    Matriz<long double> A = matriz_de_hilbert(10);

    SVD svd = fatoracao_SVD(A, true);
/*
    svd.U.print();
    cout << "\n";
    svd.Σ.print();
    cout << "\n";
    svd.Vt.print();

*/
    
    
    return 0;
}