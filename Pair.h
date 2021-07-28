#ifndef PAIR_H_MF  
#define PAIR_H_MF 1

template <typename A, typename B>
class Pair{
  
  public:
  
  Pair(const A & a, const B & b){
      first = a;
      second = b;
  }
  
  A first;
  B second;
    
};

#endif