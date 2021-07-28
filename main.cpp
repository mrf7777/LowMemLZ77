
#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>
#include "Triple.h"
#include "lz77.h"
#include "ArrayResource.h"

using namespace std;

int main()
{
    string data1;
    cin >> data1;

    const size_t bitsPerChar = 4;
    const size_t numOfChars = data1.size();

    char * data1pointer = new char[data1.length()+1];
    strcpy(data1pointer, data1.c_str());
    //do not include the null terminator
    std::size_t data1size = data1.length();
    //std::std::size_t array1size = 10;
    //char array1[array1size] = {'H', 'e', 'l', 'l', 'o', '&', 'H', 'e', 'l', 'p'};
    ArrayResource<char> res1(data1pointer, data1size);
    std::size_t res1it1 = 0;

    size_t numOfBlocks = 0;

    cout << "Pointer before loop: " << res1it1 << endl;
    while(res1it1 < res1.getSize()){
        Triple<char, std::size_t, std::size_t> bestMatch1 = lz77_getNextLDP<ArrayResource<char>, char>(res1, res1it1, 31, 63);
        cout << bestMatch1.first << " " << bestMatch1.second << " " << bestMatch1.third << endl;
        numOfBlocks++;
        cout << "Pointer end loop: " << res1it1 << endl;
    }

    const size_t bitsPerBlock = 16;


    cout << endl;
    cout << "Total uncompressed bits: " << numOfChars * bitsPerChar << endl;
    cout << "Total compressed bits: " << bitsPerBlock * numOfBlocks << endl;

    return 0;
}
