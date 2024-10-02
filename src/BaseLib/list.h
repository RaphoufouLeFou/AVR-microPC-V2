#pragma once

#include "baseLib.h"
#include "memory.h"

#define nullptr 0

template <typename T>
struct list_node
{
    T data;
    list_node *next;
    list_node *prev;
};

template <typename T>
class List {
public:
    List(){
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void push_back(T data){
        list_node<T> *node = (list_node<T>*) malloc(sizeof(list_node<T>));
        node->data = data;
        node->next = nullptr;
        node->prev = nullptr;
        if(size == 0){
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }
    void push_front(T data){
        list_node<T> *node = (list_node<T>*) malloc(sizeof(list_node<T>));
        node->data = data;
        node->next = nullptr;
        node->prev = nullptr;
        if(size == 0){
            head = node;
            tail = node;
        } else {
            head->prev = node;
            node->next = head;
            head = node;
        }
        size++;
    }

    T pop_back(){
        if(size == 0) return nullptr;
        T data = tail->data;
        list_node<T> *node = tail;
        tail = tail->prev;
        if(tail != nullptr) tail->next = nullptr;
        free(node);
        size--;
        return data;
    }

    T pop_front(){
        if(size == 0) return nullptr;
        T data = head->data;
        list_node<T> *node = head;
        head = head->next;
        if(head != nullptr) head->prev = nullptr;
        free(node);
        size--;
        return data;
    }

    T * operator[](int index){
        if(index >= size) return nullptr;
        list_node<T> *node = head;
        for(int i = 0; i < index; i++){
            node = node->next;
        }
        return &node->data;
    }

    int length(){
        return size;
    }

    void clear(){
        list_node<T> *node = head;
        while(node != nullptr){
            list_node<T> *next = node->next;
            free(node);
            node = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~List(){
        clear();
    }

private:
    list_node<T> *head;
    list_node<T> *tail;
    int size;
};

template <typename T>
class Queue {
public:
    Queue(){
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void push(T data){
        list_node<T> *node = (list_node<T>*) malloc(sizeof(list_node<T>));
        node->data = data;
        node->next = nullptr;
        node->prev = nullptr;
        if(size == 0){
            head = node;
            tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }

    T pop(){
        T data = head->data;
        list_node<T> *node = head;
        head = head->next;
        if(head != nullptr) head->prev = nullptr;
        free(node);
        size--;
        return data;
    }

    T peek(){
        return head->data;
    }

    T * operator[](int index){
        list_node<T> *node = head;
        for(int i = 0; i < index; i++){
            node = node->next;
        }
        return &node->data;
    }

    int length(){
        return size;
    }

    void clear(){
        list_node<T> *node = head;
        while(node != nullptr){
            list_node<T> *next = node->next;
            free(node);
            node = next;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~Queue(){
        clear();
    }

private:

    list_node<T> *head;
    list_node<T> *tail;
    int size;
};