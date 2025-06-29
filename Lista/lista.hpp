#ifndef LISTA_HPP
#define LISTA_HPP

#include <initializer_list>
#include <iostream>

template<typename dataT>
class node {
public:
    dataT data;
    node<dataT>* nextNode = nullptr;
    node<dataT>* previousNode = nullptr;    
    node(dataT dataParam) : data(dataParam) {}
};

template<typename dataT>
std::ostream &operator << (std::ostream &os, const node<dataT> &node) {
    os << node.data;
    return os;
}


template<typename dataT>
class list {
private:
    void deepCopy(const list &otherList) {
        node<dataT>* auxNode = otherList.head;
        while(auxNode != nullptr) {
            this->push_back(auxNode->data);
            auxNode = auxNode->nextNode;
        }
    }

    void steal(list &otherList) {
        this->head = otherList.head;
        this->tail = otherList.tail;
        this->numNodes = otherList.numNodes;

        otherList.head = nullptr;
        otherList.tail = nullptr;
        otherList.numNodes = 0;
    }

public:
    node<dataT>* head = nullptr;
    node<dataT>* tail = nullptr;
    int numNodes = 0;
    
    list() {}

    list(std::initializer_list<dataT> values) {
        for(auto &value : values) {
            push_back(value);
        }
    }

    list(const list& otherList) {
        this->deepCopy(otherList);
    }

    ~list() {
        this->clean();
    }

    class iterador {
    private:
        node<dataT>* currentNode;
    public:
        iterador(node<dataT>* node) : currentNode(node) {}

        // operador de referencia, para pegar o conteudo daquele iterador, o dataT
        dataT& operator*() const {
            return currentNode->data;
        }
        
        // operador de avançar na lista, retorna o proximo iterador ou nullptr quando o ultimo
        iterador& operator++() {
            if(currentNode != nullptr) {
                currentNode = currentNode->nextNode;
            }
            return *this;
        }

        bool operator==(const iterador& otherIterador) const {
            return currentNode == otherIterador.currentNode;
        }

        bool operator!=(const iterador& otherIterador) const {
            return !(currentNode==otherIterador.currentNode);
        }

    };

    iterador begin() {
        return iterador(head);
    }

    iterador end() {
        return iterador(tail->nextNode);
    }

    void clean() {
        node<dataT>* auxNode; 
        while (head != nullptr) {
            auxNode = head->nextNode;
            delete head;
            head = auxNode;
        }
        head=nullptr;
        tail=nullptr;
        numNodes=0;
    }

    void push_back(dataT value) {
        node<dataT>* newNode = new node<dataT>(value); 

        if(head==nullptr) { // Se a list está vazia, entao ele inicializa 
            head = newNode;
            tail = newNode;

        } else{ // Se a lista não está vazia
            newNode->previousNode = tail; // O novo ultimo node aponta para traz
            tail->nextNode = newNode; // O penultimo node aponta para frente
            tail = newNode; // aponta a tail para o novo ultimo node
        } 
        ++numNodes;  
    }
    
    void push_front();

    void pop_front();

    void pop_back();

    void erase(int index); // apaga o item deste index

    void erase(dataT data); // apaga o primeiro item com o data

    void eraseAll(dataT data). // apaga todos os elementos com data

    dataT find(int index) {}

    bool empty() {
        if(numNodes==0) return true;
        return false; 
    }

    int size() {
        return numNodes;
    }
    
    list operator = (list otherList) {
        if(this == &otherList) return *this;

        list<dataT> auxList;
        auxList->deepCopy(otherList);   

        this->clean();

        this->steal(auxList);

        return this;
    }
    
    list operator = (std::initializer_list<dataT> values) {
        list<dataT> auxList(values);
        this->clean();
        this->steal(auxList);
        return this;
    }

    bool operator < (list otherList) {
        if(this->numNodes < otherList.numNodes) return true;
        return false;
    }

    bool operator > (list otherList) {
        if(this->numNodes > otherList.numNodes) return true;
        return false;
    }

    bool operator == (int number) {
        if(this->numNodes == number) return true;
        return false;
    }

};
#endif