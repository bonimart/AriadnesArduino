template<typename T>
class Stack{

private:
    struct Elem;
    Elem * m_head;

public:
    Stack();
    ~Stack();
    void push(T val);
    T pop();
};