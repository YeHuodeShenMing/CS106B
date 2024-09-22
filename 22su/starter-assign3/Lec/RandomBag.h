#pragma once
#include "vector.h"

class RandomBag {
public :
    void add(int value);
    int removeRandom();
    int size() const;
    bool isEmpty() const;

private:
    Vector<int> elems;
};
