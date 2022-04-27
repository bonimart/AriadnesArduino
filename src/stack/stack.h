#ifndef stack_h
#define stack_h

class Stack{

private:
    struct Elem{
        float m_val;
        Elem * m_parent;
    };
    Elem * m_head;

public:
    Stack();
    ~Stack();
    void push(float val);
    float pop();
};

#endif