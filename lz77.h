#ifndef LZ77_H_MF
#define LZ77_H_MF 1

#include "Triple.h"
#include "Pair.h"
#include <cstdint>

/*
    lz77 function declarations
*/

/*
    Given a random access resource (behaves like an array) and an index which describes where the current
    element is being processed, this function will return the next length-distance pair.
    
    Calling this will also alter currentPointer to be the index of the next element for processing.
    When currentPointer==data.size() after the function call, there is no more data to be processed.
*/
template <typename RandomAccessible, typename Value>
Triple<Value, std::size_t, std::size_t> lz77_getNextLDP(
    const RandomAccessible & data,
    std::size_t & currentPointer,
    std::size_t maxLength,
    std::size_t maxDistance
    );

/*
    Returns a pair (matchLength, matchPointer).
    
    If there is no match, the pair (0, x) is returned, where x is an undefined number
*/
template <typename RandomAccessible, typename Value>
Pair<std::size_t, std::size_t> lz77_getBestMatch(
    const RandomAccessible & data,
    std::size_t currentPointer,
    std::size_t maxDistance
    );

/*
    Returns the length of the match provided.
    
    Returns 0 if the proposed match is not a match.
*/
template <typename RandomAccessible, typename Value>
std::size_t lz77_getMatchLength(
    const RandomAccessible & data,
    std::size_t currentPointer,
    std::size_t matchPointer
    );

/*
    lz77 function implementations
*/

template <typename RandomAccessible, typename Value>
std::size_t lz77_getMatchLength(
    const RandomAccessible & data,
    std::size_t currentPointer,
    std::size_t matchPointer
    ){
    
    //if match is not in a valid location
    if(matchPointer >= currentPointer){
        return 0;
    }
    //if the match location is before the currentPointer, which is valid.
    else{
        
        //used to count how long the match is.
        std::size_t counter = 0;
        
        while(data.get(matchPointer) == data.get(currentPointer) && data.isValid(currentPointer)){
            counter++;
            matchPointer++;
            currentPointer++;
        }
        
        return counter;
        
    }
    
}

template <typename RandomAccessible, typename Value>
Pair<std::size_t, std::size_t> lz77_getBestMatch(
    const RandomAccessible & data,
    std::size_t currentPointer,
    std::size_t maxDistance
    ){
    
    //if there is no input in the resource
    if(data.getSize() == 0){
        return Pair<std::size_t, std::size_t>(0, 0);
    }
    //there is at least one element in the resource
    else{
        
        //declare some working values
        std::size_t currentMatchPointer = currentPointer-1;
        std::size_t bestMatchPointer = currentMatchPointer;
        std::size_t bestMatchLength = 0;
        
        //start searching for matches
        while((currentPointer - currentMatchPointer) < maxDistance && data.isValid(currentMatchPointer)){
            //get the length of the current match
            std::size_t currentMatchLength = lz77_getMatchLength<RandomAccessible, Value>(data, currentPointer, currentMatchPointer);
            //if this match is better (longer), update the best match
            if(currentMatchLength > bestMatchLength){
                bestMatchLength = currentMatchLength;
                bestMatchPointer = currentMatchPointer;
            }
            
            //update the current match info for the next check if it happens.
            currentMatchPointer--;
        }
        
        //here ends the search for the best match.
        //the best match is found or there was no match.
        //if there was no match, bestMatchLength == 0;
        
        //report results
        return Pair<std::size_t, std::size_t>(bestMatchLength, bestMatchPointer);
    }
    
}

template <typename RandomAccessible, typename Value>
Triple<Value, std::size_t, std::size_t> lz77_getNextLDP(
    const RandomAccessible & data,
    std::size_t & currentPointer,
    std::size_t maxLength,
    std::size_t maxDistance
    ){
    
    //this case should never happen.
    //always check before calling function if the resource even has data.
    if(data.getSize() == 0){
        return Triple<Value, std::size_t, std::size_t>(Value(), 0, 0);
    }
    //consider the case where the caller wants the LDP of the first element.
    //just return a literal
    else if(currentPointer == 0){
        currentPointer++;
        return Triple<Value, std::size_t, std::size_t>(data.get(0), 0, 0);
    }
    //the LDP the caller wants is non-trivial
    else{
        //get the best match
        Pair<std::size_t, std::size_t> bestMatch = lz77_getBestMatch<RandomAccessible, Value>(data, currentPointer, maxDistance);
        
        //see if there is a match
        //first, test to see if no match was found
        if(bestMatch.first == 0){
            Triple<Value, std::size_t, std::size_t> result(data.get(currentPointer), 0, 0);
            currentPointer++;
            return result;
        }
        //there is a match
        else{
            //if the length is larger then maxLength, shrink it to fit
            if(bestMatch.first > maxDistance){
                bestMatch.first = maxDistance;
            }
            
            //this function is expected to return a Value type which
            //represents the literal which follows the match.
            //determine if the length needs to be reduced by 1 to allow a
            //literal some space.
            //this will only be necesary when the match substituion ends
            //at the end of the data.
            
            //if the end of the match substituiton does not leave one more
            //slot for the final literal of the data.
            if(currentPointer + bestMatch.first >= data.getSize()){
                //decrement the length of the match by 1 to allow space for the terminating literal.
                bestMatch.first--;
            }
            
            //calculate result
            Triple<Value, std::size_t, std::size_t> result(data.get(currentPointer+bestMatch.first), bestMatch.first, currentPointer-bestMatch.second);
            
            
            //update currentPointer
            currentPointer = currentPointer + bestMatch.first + 1;
            
            //report result
            return result;
        }
        
        
    }
    
}

#endif