#include <initializer_list>
#include <iostream>

template<typename dataT>
class node {
public:
    dataT data;
    node<dataT>* nextNode;
    node<dataT>* previousNode;    
    node(dataT dataParam) : data(dataParam), nextNode(nullptr), previousNode(nullptr) {}
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