#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include "lista.hpp"
namespace algoritmo {
    namespace {

        /**
         * @brief Dado a cabeça de duas listas é feita uma comparação node a node onde o menor node e é chamado de novo a função.
         * @param nodeA A cabeça da primeira lista.
         * @param nodeB A cabeça da segunda lista.
         */
        template<typename tipo>
        node<tipo>* merge(node<tipo>* nodeA, node<tipo>* nodeB) {
            
            node<tipo>* nodeResultado = nullptr; // Aonde será inserido os nodes já comparados

            // Caso onde algumas das listas chegou ao fim. (caso base das chamas recursivas)
            if(nodeA==nullptr) return nodeB;
            if(nodeB==nullptr) return nodeA;

            // Comparação e inserção na lista resultante.
            if(nodeA->data <= nodeB->data) { // (Este '=' torna o algoritmo estável)
                nodeResultado = nodeA;
                nodeResultado->nextNode = merge(nodeA->nextNode, nodeB);
            } else {
                nodeResultado = nodeB;
                nodeResultado->nextNode = merge(nodeA, nodeB->nextNode);
            }

            // Faço o node da frente apontar para trás (tem que deixar a lista duplamente encadeada)
            if(nodeResultado->nextNode != nullptr) nodeResultado->nextNode->behindNode = nodeResultado;
            
            nodeResultado->behindNode = nullptr; // A cabeça da lista não aponta para trás.
            
            return nodeResultado;

        }

        /**
         * @brief Separa uma lista em duas.
         * @param head_da_lista A cabeça da lista que será separada.
         * @param head_da_lista_da_frente Ponteiro da cabeça da primeira metade da lista.
         * @param head_da_lista_de_tras Ponteiro da cabeça da segunda metade da lista.
         */
        template<typename tipo>
        void splitList(node<tipo>* head_da_lista, node<tipo>** head_da_lista_da_frente, node<tipo>** head_da_lista_de_tras) {
            
            node<tipo>* node_lento = head_da_lista;
            node<tipo>* node_rapido = head_da_lista->nextNode;

            // Avança o node_rapido de dois em dois para que quando ele chegue no final o node_lento fique na metade da lista.
            while(node_rapido!=nullptr) {
                node_rapido = node_rapido->nextNode;
                if(node_rapido!=nullptr) {
                    node_rapido = node_rapido->nextNode;
                    node_lento = node_lento->nextNode;
                }
            }

            // Define as cebaças das novas listas.
            *head_da_lista_da_frente = head_da_lista; // é a head_da_lista até a metade da lista
            *head_da_lista_de_tras = node_lento->nextNode; // é a metade da lista até o final

            // Desconecta a lista e realmente forma as duas.
            node_lento->nextNode = nullptr;
            if(*head_da_lista_de_tras != nullptr) (*head_da_lista_de_tras)->behindNode = nullptr;
        }

        /**
         * @brief Ordenda uma lista (faz de fato o merge sort).
         * @param referencia_da_head_da_lista A referencia da cabeça da lista que será ordenada.
         */
        template<typename tipo>
        void merge_sort(node<tipo>** referencia_da_head_da_lista) {

            node<tipo>* head_da_lista = *referencia_da_head_da_lista;
            node<tipo>* head_da_lista_da_frente;
            node<tipo>* head_da_lista_de_tras;

            // Caso base das chamadas recursivas.
            if(head_da_lista == nullptr || head_da_lista->nextNode == nullptr) return;

            splitList(head_da_lista, &head_da_lista_da_frente, &head_da_lista_de_tras); // separa a lista em duas.
            merge_sort(&head_da_lista_da_frente); 
            merge_sort(&head_da_lista_de_tras); 

            *referencia_da_head_da_lista = merge(head_da_lista_da_frente, head_da_lista_de_tras); // junta as partes já ordenadas

        }

    }

    template<typename tipo>
    void MergeSort(Lista<tipo>& lista) {
        
        // Ordena a lista
        merge_sort(&lista.head);

        // Reorganiza a tail
        if(lista.head == nullptr) { // caso onde não há ninguem
            lista.tail = nullptr;
        } else {
            node<tipo>* aux = lista.head;
            while(aux->nextNode != nullptr) aux = aux->nextNode;
            lista.tail = aux;
        }

    }

}

#endif