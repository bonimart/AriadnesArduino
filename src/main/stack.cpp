#include "stack.h"

template<typename T>
struct Stack<T>::Elem{
    T m_val;
    Elem * m_parent;
};

template<typename T>
Stack<T>::Stack(){
    m_head = nullptr;
}

template<typename T>
Stack<T>::~Stack(){
    while(m_head){
        Elem * tmp = m_head -> m_parent;
        delete m_head;
        m_head = tmp;
    }
}

template<typename T>
void Stack<T>::push(T val){
    Elem * next = new Elem
    next -> m_val = val; 
    next -> m_parent = head;
    head = next;
}

template<typename T>
T Stack<T>::pop(){
    if(m_head){
        T val = m_head -> m_val;
        Elem * tmp = m_head -> m_parent;
        delete m_head;
        m_head = tmp;
    }
}