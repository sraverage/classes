#ifndef FUNC_MATRIZ
#define FUNC_MATRIZ

#include "matriz.hpp"
#include <string>
#include <sstream>
#include <cmath>

Matriz<long double> get_vetor(const Matriz<long double>& A, int coluna, int linha=0) {
    // copia da matriz um vetor, sobre um index na matriz.
    Matriz<long double> vetor(A.N-linha, 1);
    for(int i=0; A.N-linha>i; i++) {
        vetor[i][0] = A[i+linha][coluna];
    }
    return vetor;
}

inline long double max(const Matriz<long double>& A) {
    long double max = A[0][0];
    for(int i=0; A.N>i; i++) {
        for(int j=0; A.M>j; j++) {
            if(A[i][j]>max) max = A[i][j]; 
        }
    }
    return max;
}

inline long double norma_vetorial(const Matriz<long double>& A, long double p) {
    if(!A.eh_vetor()) throw "Só pode-se calcular a norma-vetorial-p para vetores.";

    long double sum = 0.0L;
    for(int i=0; A.N>i; i++) {
        sum += std::pow(modulo(A[i][0]), p);
    }

    return std::pow(sum, 1.0L/p);
}

inline long double norma_frobenius(const Matriz<long double>& A) {
    long double sum = 0.0;
    for(int i=0; A.N>i; i++) {
        for(int j=0; A.M>j; j++) {
            sum += A[i][j] * A[i][j];
        }
    }

    return std::sqrt(sum);
}

inline long double norma_matricial_1(const Matriz<long double>& A) { //coluna de maior norma 1
    Matriz<long double> vetor(A.M, 1);
    for(int j=0; A.M>j; j++) {
        for(int i=0; A.N>i; i++) {
            vetor[j][0] += modulo(A[i][j]);
        }
    }
    return max(vetor);
}

inline long double norma_matricial_infinita(const Matriz<long double>& A) { //linha de maior norma 1
    Matriz<long double> vetor(A.N, 1);
    for(int i=0; A.M>i; i++) {
        for(int j=0; A.N>j; j++) {
            vetor[i][0] += modulo(A[i][j]);
        }
    }
    return max(vetor); 
}

inline long double soma_do_triangulo_inferior(const Matriz<long double>& A) {
    long double soma = 0.0L;
    int limite = (A.N < A.M) ? A.N : A.M;
    for(int i=0; limite-1>i; i++) {
        Matriz<long double> v = get_vetor(A, i, i+1);
        soma += norma_vetorial(v, 1);
    }
    return soma;
}

template<typename T>
inline Matriz<T> transpor(const Matriz<T>& A) {
    // calcular a A transposta e retornar
    Matriz<T> Atrasposta(A.M, A.N); // dimenções invertidas

    for(int i=0; A.N>i; i++) {
        for(int j=0; A.M>j; j++) {
            Atrasposta[j][i] = A[i][j];
        }
    }

    return Atrasposta;
}

template<typename T>
inline const Matriz<T> produto_externo(const Matriz<T>& A, const Matriz<T>& B) {
    if(!A.eh_vetor() || !B.eh_vetor()) throw "O produto externo precisa ser de vetores";

    Matriz<T> resul(A.N, B.N);

    for(int i=0; A.N>i; i++) {
        for(int j=0; B.N>j; j++) {
            resul[i][j] = A[i][0] * B[j][0];
        }
    }

    return resul;

}

template<typename T>
inline const T produto_interno(const Matriz<T>& A, const Matriz<T>& B) {
    if(!A.eh_vetor() || !B.eh_vetor()) throw "O produto interno precisa ser de vetores";

    if(A.N != B.N) throw "O produto interno precisa que as matrizes tenham a mesma dimenção.";

    T resul{};
    for(int i=0; A.N>i; i++) {
            resul += A[i][0] * B[i][0];
        
    }

    return resul;

}

inline const Matriz<long double> matriz_de_hilbert(int ordem) {
    Matriz<long double> A(ordem);

    for(int i=0; ordem>i; i++) {
        for(int j=0; ordem>j; j++) {
            A[i][j] = (1.0L / (i+j+1.0L));
        }
    }

    return A;
}

inline const Matriz<long double> pegar_coeficientes(const std::string& polinomio) {
    
    std::string termo;
    std::stringstream ss(polinomio);

    // Pegando o primeiro coeficiente
    ss >> termo;
    std::size_t posi_x_principal = termo.find('x');
    if(posi_x_principal == std::string::npos) throw "Erro no primeiro coeficiente"; 
    std::string coeficiente_principal_str = termo.substr(0, posi_x_principal);
    long double coeficiente_maior_grau;
    if(coeficiente_principal_str.empty()) coeficiente_maior_grau = 1.0L;
    else coeficiente_maior_grau = stold(coeficiente_principal_str);
    // Pegando o primeiro expoente
    std::size_t posi_expoente_principal = termo.find('^');
    if(posi_expoente_principal == std::string::npos) throw "Erro no primeiro expoente";
    std::string expoente_principal = termo.substr(posi_expoente_principal+1);
    int maior_expoente = stoi(expoente_principal);

    Matriz<long double> coeficientes(maior_expoente, 1);

    while(ss >> termo) {
        if(termo=="+") continue;

        long double coeficiente = 0.0L;
        int expoente = 0;

        std::size_t posi_x = termo.find('x');
        std::size_t posi_potencia = termo.find('^');

        if(posi_x == std::string::npos && posi_potencia == std::string::npos) { // não possui X (termo independente)
            coeficientes[0][0] = stold(termo)/coeficiente_maior_grau;
            continue;
        }

        // Extração do coeficiente
        std::string coeficiente_str = termo.substr(0, posi_x);
        if(coeficiente_str.empty()) { // coeficiente implicito (x^3)
            coeficiente = 1.0L;
        } else if (coeficiente_str == "-") { // negatico com o coeficiente explicito (-x^2)
            coeficiente = -1.0L;
        } else {
            coeficiente = stold(coeficiente_str);
        }


        // Extração do expoente
        if(posi_potencia == std::string::npos) { // termo linear (2x)
            coeficientes[1][0] = coeficiente/coeficiente_maior_grau;
            continue;
        }

        std::string expoente_str = termo.substr(posi_potencia+1);
        expoente = stoi(expoente_str);


        coeficientes[expoente][0] = coeficiente/coeficiente_maior_grau;

    }

    return coeficientes;
}

inline Matriz<long double> matriz_companheira(std::string polinomio) {

    Matriz<long double> coeficientes = pegar_coeficientes(polinomio);

    int ordem = coeficientes.N;
    Matriz<long double> companheira(ordem);

    // Preenche a sub-diagonal com 1's
    for(int i=0; ordem-1>i; i++) {
        companheira[i+1][i] = 1.0L;
    }

    // Preenche a ultima coluna com os coeficientes *-1
    for(int i=0; ordem>i; i++) {
        companheira[i][ordem-1] = -coeficientes[i][0];
    }

    return companheira;
    
}

inline const long double eh_ortogonal(const Matriz<long double>& A) {
    // Teste da QtQ = I
    return norma_matricial_1((transpor(A) * A) - Identidade(A.M)); 
}

inline const bool eh_diagonal(const Matriz<long double>& A) {
    return true;
}

inline const long double eh_triangular_superior(const Matriz<long double>& A) {
    return soma_do_triangulo_inferior(A);

}
#endif