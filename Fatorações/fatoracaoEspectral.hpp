#ifndef FATORACAO_ESPECTRAL
#define FATORACAO_ESPECTRAL

#include "matriz.hpp"
#include "funcoesMatriz.hpp"
#include "fatoracaoQR.hpp"
#include "constantes.hpp"

#include <iostream>
#include <cmath>

class QΛQ {
public:
    Matriz<long double> Q;
    Matriz<long double> Λ;
    Matriz<long double> A;
    Matriz<long double> Qt;
    QΛQ(const Matriz<long double>& q, const Matriz<long double>& espec, const Matriz<long double>& a) :
    Q(q), Λ(espec), A(a), Qt(transpor(q)) {}
};

inline const long double calculo_do_shift(const Matriz<long double>& A, int n, long double norma_da_linha) {
    long double a, b, c, distancia, denominador;

    a = A[n-1][n-1];
    b = modulo(A[n][n-1]);
    c = A[n][n];

    distancia = (a-c)/2;
    denominador = modulo(distancia) + sqrtl((distancia*distancia) + (b*b));

    long double shifter = c;
    if(denominador>1e-17L) {
        long double sinal = (distancia>=0) ? 1.0L : -1.0L;
        shifter = c - ((sinal*(b*b))/denominador);   
    }

    if(modulo(shifter) <= erro && norma_da_linha > 1e-7L) {
        return norma_da_linha;
    } else return shifter;

}

inline const long double norma_linha(const Matriz<long double>& A, int coluna) {
    long double soma=0.0L;

    for(int i=0; coluna>i; i++) {
        soma += modulo(A[coluna][i]);
    }

    return soma;
}

class Hessenberg {
public:
    Matriz<long double> H;
    Matriz<long double> Q;
    Hessenberg(const Matriz<long double>& t, const Matriz<long double>& q) :
    H(t), Q(q) {}
};

inline const Hessenberg hessenberg(const Matriz<long double>& A) {
    Matriz<long double> hessen = A;
    int ordem = A.N;
    Matriz<long double> Q = Identidade(ordem);

    for(int j=0; ordem-2>j; j++) {
        Matriz<long double> vetor = get_vetor(hessen, j, j+1);
        Matriz<long double> refletor = construtor_do_refletor(vetor, ordem);

        hessen = (refletor * hessen * refletor);
        Q = Q*refletor;

    }

    hessen.print();

    return Hessenberg(hessen, Q);

}

inline const QΛQ fatoracao_espectral(const Matriz<long double>& A, bool print=false) {
    if(!A.eh_quadrada()) {    
        std::cout << "Não se pode fazer fatoração espetral de uma matriz não quadrada.\n";
        throw "Erro";
    } else if(!A.eh_simetrica()) {
        std::cout << "Não se pode fazer fatoração espetral real em uma matriz não simetrica.\n";
        throw "Erro";
    }

    Hessenberg hessen = hessenberg(A);
    Matriz<long double> espectro = hessen.H;
    Matriz<long double> Q = hessen.Q;
    Matriz<long double> I = Identidade(A.N);
    QR aux;

    int num_fatoracoes_global = 0;
    int num_fatoracoes_local = 0;
    int max_iteracoes_por_autovalor = limite_de_fatoracoes*A.N;

    for(int m = A.N-1; m>0; m--) { // Faz cada um dos N autovalores convergirem por vez
        num_fatoracoes_local = 0;

        while(true) {
            long double norma_lin = norma_linha(espectro, m);
            if(norma_lin <= erro) break; // Condição de parada
            
            long double shift = 42;//calculo_do_shift(espectro, m, norma_lin);
            //std::cout << "Shifter: " << shift << "\n";
            Matriz<long double> matriz_deslocada = espectro - (I*shift);
            aux = fatoracao_QR(matriz_deslocada, false);
            espectro = (aux.R * aux.Q) + (I*shift);

            Q = Q*aux.Q;

            num_fatoracoes_global++;
            num_fatoracoes_local++;

            if(num_fatoracoes_local>max_iteracoes_por_autovalor) {
                std::cout << "\nUm dos autovalores teve o seu número máximo de iterações alcançado. Pulando para o proximo.";
                espectro[m][m-1] = 0;
                break;
            }

            if(!espectro.eh_simetrica()) {
                std::cout << "Não é simetrica\n";
            }

        }
    }
    
    QΛQ resposta(Q, espectro, A);

    if(print)std::cout << "\nNumero de iterações: " << num_fatoracoes_global << "\n \n";
    
    return resposta;
}



#endif