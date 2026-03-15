#ifndef LISTA_HPP
#define LISTA_HPP

#include <iostream>

/**
 * @struct node
 * @brief Representa um nó de uma lista duplamente encadeada.
 */
template<typename tipo>
struct node {
    node<tipo>* nextNode = nullptr;
    node<tipo>* behindNode = nullptr;
    tipo data;
};

// Declara a lista antes e a insere no namespace do mergesort.
template<typename T> class Lista;
namespace algoritmo {
    template<typename T> void MergeSort(Lista<T>& lista);
}


/**
 * @class lista
 * @brief Uma lista duplamente encadeada.
 */
template<typename tipo>
class Lista {
private:
    node<tipo>* head = nullptr; // Ponteiro da cabeça
    node<tipo>* tail = nullptr; // Ponteiro da cauda
    int tamanho=0;

    friend void algoritmo::MergeSort<>(Lista<tipo>& lista);

public:

public:
    Lista() = default; 
    
    Lista(const Lista<tipo>& outra) : head(nullptr), tail(nullptr), tamanho(0) {

        // Copia elemento por elemento.
        for (const node<tipo>* aux = outra.begin(); aux!=outra.end(); aux=aux->nextNode) {
            this->push_back(aux->data);
        }
    }

    Lista<tipo>& operator=(const Lista<tipo>& outra) {
        if (this == &outra) { //Evita auto atribuição.
            return *this;
        }

        this->clear(); 

        // Copia a lista.
        for (const node<tipo>* aux = outra.begin(); aux!=outra.end(); aux = aux->nextNode) {
            this->push_back(aux->data);
        }

        return *this;
    }

    node<tipo>* searchNode(int index) {
        
        // Nestes casos não deve haver a busca.
        if(tamanho==0 || index<1 || index>tamanho) return nullptr;

        if(index==1) return head;
        if (index==tamanho) return tail;

        node<tipo>* aux;
        if((index-1)<=(tamanho-index)) { // Neste caso, buscar pela cabeça é mais rapido.
            aux = head;
            for(int i=1; i<index; ++i){
                aux = aux->nextNode;
            }

        } else { // Neste caso, buscar pela cauda é mais rapido.
            aux = tail;
            for(int i=tamanho; i>index; i--) {
                aux = aux->behindNode;
            }
        }

        return aux;
    }

    ~Lista() {
        this->clear(); // Esta função destroi e limpa a lista.
    }

    tipo* get(int index) const {
        node<tipo>* aux = searchNode(index);
        if(aux!=nullptr) return &(aux->data);
        return nullptr;
    }

    void set(int index, tipo objeto) {
        if(index<=0 || index>tamanho) return;
        searchNode(index)->data = objeto;
    }

    node<tipo>* search(tipo objeto) {
        node<tipo>* aux = head; 
        
        while(aux != nullptr) { // É preciso olhar node por node.
            if(aux->data == objeto) return aux;
            aux = aux->nextNode;
        }
        return nullptr;
    }

    int get_size() const {
        return tamanho;
    }

    void push_front(const tipo& objeto) {
        push(1, objeto); // Esta lista começa pelo index 1.

    }
    
    void push_back(const tipo& objeto) {
        push(tamanho+1, objeto);
    }

    void push(int index, const tipo& objeto) {
        if(index<=0 || index>tamanho+1) return;

        node<tipo>* newNode = new node<tipo>;
        newNode->data = objeto;

        if(tamanho==0) {
            head = newNode;
            tail=newNode;
        } else if (index==1) {
            newNode->nextNode = head;
            head->behindNode = newNode;
            head = newNode;
        } else if (index==tamanho+1) {
            tail->nextNode = newNode;
            newNode->behindNode = tail;
            tail = newNode;
        } else {
            node<tipo>* index_node = searchNode(index);
            node<tipo>* behind_node = index_node->behindNode;

            behind_node->nextNode = newNode;
            newNode->behindNode = behind_node;

            newNode->nextNode = index_node;
            index_node->behindNode = newNode;
        }
        tamanho++;

    }

    void push(node<tipo>* node_index, const tipo& objeto) {
        
        if(node_index==nullptr) return;

        node<tipo>* new_node = new node<tipo>;
        new_node->data = objeto;
        
        node<tipo>* behind_node = node_index->behindNode;

        if(node_index==head) { // Caso onde o node de tras não existe.
            node_index->behindNode = new_node;
            new_node->nextNode = node_index;
            head = new_node;
            return;
        } 

        // Ajusta o node de tras.
        behind_node->nextNode = new_node;
        new_node->behindNode = behind_node;

        // Ajusta o node do index (da frente).
        new_node->nextNode = node_index; 
        node_index->behindNode = new_node;

        tamanho++;        
    }

    tipo remove_front() {
        return remove(1);
    }

    tipo remove_back() {
        return remove(tamanho);
    }

    tipo remove(int index) {
        // Nestes casos não se deve remover.
        if(tamanho==0 || index<1 || index>tamanho) return tipo();

        node<tipo>* aux = searchNode(index);
        node<tipo>* next_aux = aux->nextNode;
        node<tipo>* behind_aux = aux->behindNode;
        tipo aux_data = aux->data;

        if(next_aux!=nullptr) next_aux->behindNode = behind_aux; // Se existir o um proximo node, ele tem que apontar para trás. 
        if(behind_aux!=nullptr) behind_aux->nextNode = next_aux; // Se existir o um node anterior, ele tem que apontar para frente.

        // Para os casos onde a cabeça ou a cauda é removida. 
        if(aux==head) head = head->nextNode;
        if(aux==tail) tail = tail->behindNode;

        tamanho--;
        delete aux;
        return aux_data;
    }

    void remove(tipo objeto) {
        node<tipo>* node_para_remover = nullptr;

        // Procura o nó que contém o objeto
        for (node<tipo>* aux = head; aux != nullptr; aux = aux->nextNode) {
            if (aux->data == objeto) {
                node_para_remover = aux;
                break;
            }
        }

        // Se não encontrou o objeto, não faz nada
        if (node_para_remover == nullptr) {
            return;
        }

        // Pega os vizinhos do no
        node<tipo>* behind_node = node_para_remover->behindNode;
        node<tipo>* next_node = node_para_remover->nextNode;

        // Refaz as conexões para "pular" o nó a ser removido
        if (behind_node != nullptr) {
            behind_node->nextNode = next_node;
        } else { // Se não há anterior, o nó a ser removido era a cabeça
            head = next_node;
        }

        if (next_node != nullptr) {
            next_node->behindNode = behind_node;
        } else { // Se não há próximo, o nó a ser removido era a cauda
            tail = behind_node;
        }

        delete node_para_remover;
        tamanho--;
    }
    
    void remove(node<tipo>* node_index) {
        if(node_index==nullptr) return;

        node<tipo>* next_node = node_index->nextNode; 
        node<tipo>* behind_node = node_index->behindNode;

        // Possiveis casos onde o node_index pode estar
        if(behind_node == nullptr && next_node != nullptr) {
            head = next_node;
            head->behindNode = nullptr;
        } else if (behind_node != nullptr && next_node == nullptr) {
            tail = behind_node;
            tail->nextNode = nullptr;
        } else if (behind_node == nullptr && next_node == nullptr) {
            head = nullptr;
            tail = nullptr;
        } else {
            behind_node->nextNode = next_node;
            next_node->behindNode = behind_node;
        }
        tamanho--;
        delete node_index;

    }

    void clear() {
        node<tipo>* aux;
        while(head != nullptr) { // Passa deletando todos os nodes
            aux = head;
            head = head->nextNode;
            delete aux;
        }
        tail=nullptr;
        tamanho=0;
    }

    void print() const {
        node<tipo>* aux = head;
        for(int i=0; tamanho>i; i++) { // É preciso passar por todos os nodes.
            std::cout << aux->data << "  ";
            aux = aux->nextNode;
        }
        std::cout << std::endl;
    }

    bool is_empty() const {
        return (tamanho==0);
    }

    void inverter() {

        if(head == nullptr) return;

        // Inverter todos os ponteiros da lista

        node<tipo>* prox = nullptr;
        node<tipo>* atual = head;
        
        while(atual != nullptr) {
            prox = atual->nextNode;
            node<tipo>* aux = atual->behindNode;
            atual->behindNode = atual->nextNode;
            atual->nextNode = aux;
            atual = prox;
        }

        // Inverter as ponteiros de ponta
        node<tipo>* aux = head;
        head = tail;
        tail = aux;

    }
 
    // Funções para facilitar a iteração
    node<tipo>* begin() {return head;}
    node<tipo>* end() {return nullptr;}

    node<tipo>* get_tail() const {
        return tail;
    }

    const node<tipo>* begin() const {return head;}
    const node<tipo>* end() const {return nullptr;}

};
#endif