#include "stack.h"

Stack::Stack(){
    m_head = nullptr;
}

Stack::~Stack(){
    while(m_head){
        Elem * tmp = m_head -> m_parent;
        delete m_head;
        m_head = tmp;
    }
}

void Stack::push(float val){
    Elem * next = new Elem;
    next -> m_val = val; 
    next -> m_parent = m_head;
    m_head = next;
}

float Stack::pop(){
    if(m_head){
        float val = m_head -> m_val;
        Elem * tmp = m_head -> m_parent;
        delete m_head;
        m_head = tmp;
        return val;
    }
}