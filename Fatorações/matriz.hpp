#ifndef matriz_hpp
#define matriz_hpp

#include <iostream>
#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include "constantes.hpp"

template<typename T>
T modulo(T valor) {
    if(valor<0) return -valor;
    return valor;
}

template<typename T>
class Matriz {
protected:
    T** matriz = nullptr;

    void clear() {

        if(matriz==nullptr) return;

        // deleta as linhas
        for(int i=0; N>i; i++) {
            delete[] matriz[i];
        }

        //deleta a coluna original
        delete[] matriz;

        N = 0;
        M = 0;
        matriz = nullptr;
    }

public:
    int N = 0;
    int M = 0;

    // CONSTRUTORES

    ~Matriz() {
        clear();
    }

    Matriz() {
        // Como você já inicializou N=0, M=0 e matriz=nullptr na declaração das variáveis
        // protected, este construtor pode ficar vazio ou apenas garantir:
        N = 0;
        M = 0;
        matriz = nullptr;
    }

    Matriz(int n, int m) : N(n), M(m) {

        matriz = new T*[N];

        for(int i=0; N>i; i++) {
            matriz[i] = new T[M]{};
        }
    }

    Matriz(int n) : Matriz(n, n) {}

    Matriz(const Matriz& B) {
        N = B.N;
        M = B.M;

        // Alocação
        matriz = new T*[N];
        for(int i=0; N>i; i++) {
            matriz[i] = new T[M]{};
        }

        // Copia
        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                matriz[i][j] = B[i][j];
            }
        }    
    }

    Matriz(std::initializer_list<std::initializer_list<T>> lista) {
        // 1. Define o número de linhas
        N = lista.size();

        // Caso a lista venha vazia: {}
        if (N == 0) {
            M = 0;
            matriz = nullptr;
            return;
        }

        // 2. Define o número de colunas baseado na primeira linha
        M = lista.begin()->size();

        // 3. Validação: Garante que não é uma matriz "denteada" (jagged array)
        for (const auto& linha : lista) {
            if (linha.size() != M) {
                // Limpa N para não deixar o objeto em estado inválido antes de lançar erro
                N = 0; M = 0; 
                throw std::invalid_argument("Erro de Inicialização: Todas as linhas da matriz devem ter o mesmo tamanho.");
            }
        }

        // 4. Alocação (padrão T**)
        matriz = new T*[N];
        for (int i = 0; i < N; i++) {
            matriz[i] = new T[M]{};
        }

        // 5. Cópia dos dados da lista para a memória alocada
        int i = 0;
        for (const auto& linha_lista : lista) {
            int j = 0;
            for (const auto& elemento : linha_lista) {
                matriz[i][j] = elemento;
                j++;
            }
            i++;
        }
    }

    Matriz& operator =(const Matriz& B) {

        if(this == &B) return *this; // Evita autoatribuição

        this->clear();
        N = B.N;
        M = B.M;

        // Realocação
        matriz = new T*[N];
        for(int i=0; N>i; i++) {
            matriz[i] = new T[M]{};
        }

        // Copia
        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                matriz[i][j] = B[i][j];
            }
        }

        return *this;
        
    }

    // SOBRECARGAS

    Matriz operator +(const Matriz& B) const {
        if(N != B.N || M != B.M) {
            std::cout << "Não se pode somar matrizes de tamanhos diferentes";
            throw "Não se pode somar matrizes de tamanhos diferentes";
        }
        Matriz<T> aux(N, M);

        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                aux[i][j] = matriz[i][j] + B[i][j]; 
            }
        } 

        return aux;

    }

    Matriz operator -(const Matriz& B) const {
        return *this + (B * -1);
    }

    Matriz operator *(double a) const {
        Matriz<T> aux(N, M);

        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                aux[i][j] = matriz[i][j] * a; 
            }
        }

        return aux;
     }

    Matriz operator /(double a) const {
        return *this * (1/a);
    }

    Matriz operator *(const Matriz& B) const {
        // Checar se é possivel multiplicar

        if(M != B.N) throw "Operacao de multiplicacao de matriizes invalida.";

        Matriz<T> aux(N, B.M);

        for(int i=0; N>i; i++) {
            for(int j=0; B.M>j; j++) {
                T soma{};
                for(int c=0; M>c; c++) soma += matriz[i][c] * B[c][j];
                aux[i][j] = soma;
            }
        }

        return aux;

    }

    const bool operator ==(const Matriz& B) const {
        if(M != B.M || N != B.N) return false;

        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                if(modulo(matriz[i][j] - B[i][j]) >= erro) return false;
            }
        }

        return true;
    }

    // ACESSOS

    T* operator [](int index) {
        return matriz[index];
    }

    const T* operator [](int index) const {
        return matriz[index];
    }

    // FUNÇÕES  

    const bool eh_simetrica() const {

        Matriz<T> transposta(M, N); // dimenções invertidas

        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                transposta[j][i] = matriz[i][j];
            }
        }

        if(*this == transposta) return true;
        return false;
    }

    const bool eh_vetor() const {
        if(M == 1) return true;
        return false;
    }

    const bool eh_quadrada() const {
        if(N == M) return true;
        return false;
    }

    Matriz coluna(int j) {
        if(N==0 || M==0 || matriz==nullptr) throw "Tentativa invalida de se pegar uma coluna de uma matriz.\n";

        Matriz<T> coluna(N, 1);
        for(int i=0; N>i; i++) {
            coluna[i][0] = matriz[i][j];
        }
        return coluna;
    }

    const bool is_empyt() {
        if(M==0 || N==0 || matriz == nullptr) return true;
        return false;
    }

    // OUTPUT

    void print() {
        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                //Antes de printar é preciso ver se o valor é pequeno.
                if(modulo(matriz[i][j]) < 1e-11L) std::cout << '0' << ' ';
                else std::cout << matriz[i][j] << ' ';
            }
            std::cout << "\n";
        }
    }

};

class Identidade : public Matriz<long double> {
public: 
    Identidade(int ordem) : Matriz(ordem) {

        for(int i=0; N>i; i++) {
            for(int j=0; M>j; j++) {
                if(i==j) matriz[i][j] = 1;
                else matriz[i][j] = 0;
            }
        }

    }
};


#endif