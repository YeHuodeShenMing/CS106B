#pragma once

#include <ostream>

using namespace std;

class OurVector{
public :
    OurVector();
    ~OurVector();

    void add(int value);
    void insert(int index, int value);
    int get(int index);
    void remove(int index);
    int size() const;
    bool isEmpty() const;

    void printDebugInfo();
    void expand();

private:
    int* elements;
    int allocatedCapacity;
    int numItems;

};
