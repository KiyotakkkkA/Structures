#ifndef __LINKEDLIST_HPP
#define __LINKEDLIST_HPP

#include <iostream>

template <typename T>
class List 
{
    class Node
    {
        public:
            T data;
            Node *next_node;

            Node(T data = T(), Node *next_node = nullptr){
                this -> data = data;
                this -> next_node = next_node;
            };
    };

    public:
        Node *head;
        int size;

        List();
        ~List();

        List& insert(const int index, T data);
        List& append(T data);
        List& append_front(T data);
        List& pop_front() noexcept;
        List& pop_back() noexcept;
        List& drop() noexcept;
        List& remove(const int index);

        int length() const;

        void print() const noexcept;

        T& operator[] (const int index);
};

template <typename T>
T& List<T>::operator[] (const int index){

    if (index >= size){
        throw std::out_of_range("ERROR! List index out of range");
    }
    
    Node *current = this -> head;
    int i = 0;
    while (current != nullptr) {
        if (i == index) {
            return current -> data;
        }

        current = current -> next_node;
        i++;
    }
}

template <typename T>
List<T>::List(){
    size = 0;
    head = nullptr;
}

template <typename T>
List<T>& List<T>::drop() noexcept{
    while (size){
        pop_front();
    }

    return *this;
}

template <typename T>
List<T>::~List(){
    drop();
}

template <typename T>
List<T>& List<T>::pop_front() noexcept{
    Node *temp = head;
    head = head -> next_node;
    size--;
    delete temp;
    return *this;
}

template <typename T>
List<T>& List<T>::pop_back() noexcept{
    remove(size - 1);
    return *this;
}

template <typename T>
List<T>& List<T>::insert(const int index, T data){
    if (index > size){
        throw std::out_of_range("ERROR! List index out of range");
    }

    Node *new_node = new Node(data);

    if (index == 0){
        new_node -> next_node = head;
        head = new_node;
        size++;
        return *this;
    }
    
    Node *current = this -> head;
    int i = 0;
    while (current != nullptr) {
        if (i == index - 1) {
            new_node -> next_node = current -> next_node;
            current -> next_node = new_node;
            break;
        }

        current = current -> next_node;
        i++;
    }
    size++;
    return *this;
}

template <typename T>
List<T>& List<T>::append_front(T data){
    insert(0, data);
    return *this;
}


template <typename T>
List<T>& List<T>::append(T data){
    if (head == nullptr){
        head = new Node(data);
    }
    else
    {
        Node *current = this -> head;
        while (current -> next_node != nullptr){
            current = current -> next_node;
        }

        current -> next_node = new Node(data);
    }

    size++;
    return *this;
}

template <typename T>
int List<T>::length() const{
    return size;
}

template <typename T>
List<T>& List<T>::remove(const int index){
    if (index > size){
        throw std::out_of_range("ERROR! List index out of range");
    }

    if (index == 0){
        pop_front();
        return *this;
    }

    Node *current = this -> head;
    Node *temp;
    int i = 0;
    while (current != nullptr) {
        if (i == index - 1) {
            temp = current -> next_node;
            current -> next_node = temp -> next_node;
            delete temp;
            size--;
            break;
        }

        current = current -> next_node;
        i++;
    }

    return *this;
}

template <typename T>
void List<T>::print() const noexcept {
    Node *current = this -> head;
    std::cout << current -> data;
    while (current -> next_node != nullptr){
        current = current -> next_node;
        std::cout << " -> " << current -> data;
    }
    std::cout << std::endl;
}

#endif