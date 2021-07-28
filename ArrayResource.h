#ifndef ARRAY_RESOURCE_H_MF
#define ARRAY_RESOURCE_H_MF 1

#include <cstdint>

template <typename T>
class ArrayResource{
    
    public:
    
    ArrayResource(T * inData, std::size_t inSize){
        data = inData;
        size = inSize;
    }
    
    bool isValid(std::size_t index) const {
        return index < size;
    }
    
    T get(std::size_t index) const {
        return data[index];
    }

    std::size_t getSize() const {
        return size;
    }
    
    private:
    
    T * data;
    std::size_t size;
    
};

#endif