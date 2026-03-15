#ifndef FAT_QR
#define FAT_QR

#include "matriz.hpp"
#include "funcoesMatriz.hpp"

// Passa-se o vetor U e ele constroi o refletor de hh, I-2UU.
Matriz<long double> refletorHH(const Matriz<long double>& vetor) {
    
    if(!vetor.eh_vetor()) throw "Para criar um refletor de HH é preciso ser um vetor.";
    
    Identidade I(vetor.N);

    // F = I - 2uu^t
    Matriz refletor = I + (produto_externo(vetor, vetor) * -2);
    
    return refletor;

}

inline long double sinal(long double a) {
    if(a>=0) return 1;
    else return -1;
}

// Passa-se o vetor V, ele encontra o U e reflete e retorna o refletor.
inline Matriz<long double> construtor_do_refletor(const Matriz<long double>& v, int num_linhas_original) {
    if(!v.eh_vetor()) throw "V precisa ser um vetor para se fabricar um refletor";
    
    // Construindo o vetor R (a reflexão)
    Matriz<long double> r(v.N, 1);
    r[0][0] = norma_vetorial(v, 2);;
    for(int i=1; v.N>i; i++) r[i][0] = 0;

    // Construindo U (v-r / ||v-r||)
    Matriz<long double> u = v + (r * sinal(v[0][0]));
    u = u / norma_vetorial(u, 2);
    
    Matriz<long double> refletor = refletorHH(u); 

    // Agora precisamos montar o refletor integral, para q ele multiplcado pela A,
    // conserve as colunas já trabalhadas e reflita a proxma coluna, assim, como todo 
    // F é quadrado, na ordem das linhas de A, vamos expandir o refletor (identidade).
    
    Identidade F(num_linhas_original);

    // Realiza uma sobrescrita do refletor em cima do refletor grande.
    int l=0, c=0;
    for(int i = (F.N - refletor.N); F.N>i; i++) {

        for(int j = (F.M - refletor.M) ; F.M>j; j++) {
            F[i][j] = refletor[l][c];
            c++;
        }

        c = 0;
        l++;
    }

    return F;

}

class QR {
public:
    Matriz<long double> A;
    Matriz<long double> Q;
    Matriz<long double> R;
    QR() {};
    QR(const Matriz<long double> a, const Matriz<long double>& q, const Matriz<long double>& r) : A(a), Q(q), R(r) {}

    const bool testar_fatoracao(bool output = true) {

        if(A.is_empyt()) {
            if(output) std::cout << "Não é possivel testar a fatoração QR se a matriz A não foi passada como parametro.\n";
            return false;
        }

        bool deu_certo = true;

        long double erroQ = eh_ortogonal(Q); 
        if(erroQ >= erro) {
            if(output)std::cout << "A fatoração QR gerou uma Q não ortogonal. Erro de "
            << erroQ << "\n";
            deu_certo = false;
        }

        long double erroR = eh_triangular_superior(R);
        if(erroR >= erro) {
            if(output)std::cout << "A fatoração QR gerou uma R que não é triangular superior. Erro de "
            << erroR << "\n";
            deu_certo = false;         
        } 

        long double erroQR = norma_matricial_1(A - (Q*R));
        if(erroQR >= erro) {
            if(output)std::cout << "A fatoração QR gerou muito erro na reconstrução QR. Erro de "
            << erroQR << "\n";
            deu_certo = false;
        }

        long double erro_F = modulo(norma_frobenius(A)-norma_frobenius(R));
        if(erro_F > erro) {
            if(output)std::cout << "A fatoração QR gerou uma R com a norma-F muito diferente de A. Erro de " << 
            erro_F << "\n";
            deu_certo = false;          
        }

        return deu_certo;
    }

};

inline const QR fatoracao_QR(const Matriz<long double>& A, bool tratamento_de_erros = false) {

    Matriz<long double> R = A;
    Matriz<long double> F = Identidade(A.N);

    int limite = (A.M < A.N-1) ? A.M : A.N-1;
    for(int j=0; limite>j; j++) { // percorre todas as colunas, menos a ultima
        Matriz<long double> v = get_vetor(R, j, j);
        
        Matriz<long double> F_aux = construtor_do_refletor(v, A.N);
        
        R = F_aux * R;
        F = F_aux * F;

    }

    QR resul(A, transpor(F), R);

    return resul;
}

inline const long double numero_de_condicionamento_QR(const QR& qr) {
    
    Matriz<long double> R = qr.R;

    long double maiorR = modulo(R[0][0]);
    long double menorR = modulo(R[0][0]);

    int limite;
    if(R.N>R.M) limite = R.M;
    else limite = R.N;

    for(int i=0; limite>i; i++){
        long double valor = modulo(R[i][i]);
        if(maiorR<valor) maiorR = valor;
        if(menorR>valor) menorR = valor;
    }

    return maiorR/menorR;


}

inline const long double numero_de_condicionamento_QR(const Matriz<long double>& A) {
    QR aux = fatoracao_QR(A);
    return numero_de_condicionamento_QR(aux);
}   


#endif