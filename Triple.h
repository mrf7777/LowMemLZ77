#ifndef TRIPLE_H_MF
#define TRIPLE_H_MF 1

template <typename A, typename B, typename C>
class Triple{
    
    public:
    
    //lvalue constructor
    Triple(const A & a, const B & b, const C & c){
        first = a;
        second = b;
        third = c;
    }
    
    A first;
    B second;
    C third;
    
};

#endif