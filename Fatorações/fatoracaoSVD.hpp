#ifndef FATORACAO_SVD
#define FATORACAO_SVD

#include "matriz.hpp"
#include "fatoracaoQR.hpp"
#include "fatoracaoEspectral.hpp"
#include "lista.hpp"
#include "mergeSort.hpp"
#include <cmath>
#include "constantes.hpp"


class SVD {
public:
    Matriz<long double> A;
    Matriz<long double> U;
    Matriz<long double> Σ;
    Matriz<long double> Vt;

    const bool testar_fatoracao() {
        bool deu_certo = true;

        long double erro_reconstrucao = norma_matricial_1(A-(U*Σ*Vt));
        if(erro_reconstrucao >= erro) {
            std::cout << "A fatoração SVD gerou muito erro na reconstrução. Erro de " 
            << erro_reconstrucao << "\n";
            deu_certo = false;
        }

        long double norma_1_U = eh_ortogonal(U);
        if(norma_1_U >= erro) {
            std::cout << "A fatoração SVD gerou uma U não ortogonal. Erro de " 
            << norma_1_U << "\n"; 
            deu_certo = false;         
        }

        long double norma_1_Vt = eh_ortogonal(U);
        if(norma_1_Vt >= erro) {
            std::cout << "A fatoração SVD gerou uma Vt não ortogonal. Erro de " 
            << norma_1_Vt << "\n"; 
            deu_certo = false;         
        }

        if(!eh_diagonal(Σ)) {
            std::cout << "A fatoração SVD gerou uma Σ não diagonal\n"; 
            deu_certo = false;         
        }

        return deu_certo;
    }
};

class AutoPar {
public:
    long double auto_valor;
    int index_coluna;

    AutoPar() : auto_valor(0.0L), index_coluna(0) {}
    AutoPar(long double v, int i) : auto_valor(v), index_coluna(i) {}

    bool operator<=(const AutoPar& outro) const {
        return this->auto_valor <= outro.auto_valor;
    }

    bool operator>(const AutoPar& outro) const {
        return this->auto_valor > outro.auto_valor;
    }

};

inline const Matriz<long double> matriz_aumentada(const Matriz<long double>& R) {
    Matriz<long double> H(2*R.N);
    Matriz<long double> Rt = transpor(R);

    // Copiar a R-transposta
    for(int i=0; R.N>i; i++) {
        for(int j=R.N; 2*R.N>j; j++) {
            H[i][j] = Rt[i][j-R.N];
        }
    }

    // Copiar a R
    for(int i=R.N; 2*R.N>i; i++) {
        for(int j=0; R.N>j; j++) {
            H[i][j] = R[i-R.N][j];
        }
    }

    return H;
}

inline const SVD recorde_da_espectral_H(const QΛQ& espectral) {
    int ordem = espectral.Q.N/2;

    Matriz<long double> U(ordem);
    Matriz<long double> Σ(ordem);
    Matriz<long double> V(ordem);

    Lista<AutoPar> autopares;

    // Coleta dos autovalores
    for(int k=0; 2*ordem>k; k++) {
        long double valor = espectral.Λ[k][k];
        if(valor > erro) autopares.push_back({valor, k});
    }

    algoritmo::MergeSort(autopares);
    autopares.inverter();

    // Preenchimento mapeado da SVD
    int k=0;
    for(node<AutoPar>* it = autopares.begin(); it!=autopares.end(); it=it->nextNode) {

        // Conferi o limite
        if(k>=ordem) break; 

        AutoPar autopar = it->data;
        // Caso a matriz tenha posto incompleto, e algum autovalor seja 0
        
        Σ[k][k] = autopar.auto_valor;

        // Preenchimento do U e V
        for(int i=0; ordem>i; i++) {
            V[i][k] = espectral.Q[i][autopar.index_coluna];
            U[i][k] = espectral.Q[i+ordem][autopar.index_coluna];
        }

        k++;
    }

    SVD resposta;
    resposta.U = U*raiz2;
    resposta.Σ = Σ;
    resposta.Vt = transpor(V)*raiz2;
    resposta.A = espectral.A;

    return resposta;
}

inline const SVD fatoracao_SVD(const Matriz<long double>& A, bool tratamento_de_erro=false) {
    QR qr = fatoracao_QR(A);
    Matriz<long double> H = matriz_aumentada(qr.R);
    QΛQ espectral_H = fatoracao_espectral(H); // unica parte iterativa
    SVD svd = recorde_da_espectral_H(espectral_H);
    svd.U = qr.Q * svd.U;
    svd.A = A;
    if(tratamento_de_erro) svd.testar_fatoracao();
    return svd;
} 

#endif