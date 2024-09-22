#include "OurVector.h"
#include "error.h"
#include "SimpleTest.h"

#define ARRAY_SIZE 8

OurVector::OurVector(){
    allocatedCapacity = ARRAY_SIZE;
    elements = new int[allocatedCapacity];
    numItems = 0;
}

OurVector::~OurVector(){
    delete[] elements;
}

// append value at end
void OurVector::add(int value){
    if (numItems == allocatedCapacity) {
        expand();
    }
    elements[numItems] = value;
    numItems += 1;
}

// insert a value at index
void OurVector::insert(int index, int value) {
    if (index < size() && size() < allocatedCapacity) {
        for (int i = size(); i > index ;i--) {
            elements[i] = elements[i - 1];
        }
    }
    elements[index] = value;

    numItems += 1;
}

// return value at index
int OurVector::get(int index){
    return elements[index];
}

// remove value at index
void OurVector::remove(int index){
    if (index < size()) {
        for (int i = index; i < size() - 1; i++) {
            elements[i] = elements[i + 1];
        }
        numItems -= 1;
    }
}

// return the number of elements in the array
int OurVector::size() const{
    return numItems;
}

// true if zero elements
bool OurVector::isEmpty() const{
    return size() == 0;
}

void OurVector::printDebugInfo(){
    cout << "Num items: " << numItems << " Allocated capacity: " << allocatedCapacity << endl;
    cout << "{";
    for (int i = 0; i < numItems; i++) {
        cout << i;
        if (i != numItems - 1) cout << ", ";
    }
    cout << "}" << endl;
}

void OurVector::expand() {
    // 1. Create a new, larger array. Usually we choose to double the current size.
    int* newElements = new int[allocatedCapacity * 2];
    // 2. Copy the old array elements to the new array.
    for (int i = 0; i < numItems; i++) {
        newElements[i] = elements[i];
    }

    // 3. Delete (free) the old array.
    delete[] elements;

    // 4. Point the old array variable to the new array.
    elements = newElements;

    // 5. Update the associated capacity variable for the array.
    allocatedCapacity *= 2;
}

/************TEST_CASE******************/

PROVIDED_TEST("Construct vector and see constructor/destructor be called."){
    OurVector vec;
}

PROVIDED_TEST("Check that newly created vector is empty"){
    OurVector vec;
    EXPECT_EQUAL(vec.isEmpty(), true);
}

PROVIDED_TEST("Add 5 elements to vector."){
    OurVector vec;
    for (int i = 0; i < 5; i++){
        vec.add(i);
    }

    EXPECT_EQUAL(vec.size(), 5);
    EXPECT_EQUAL(vec.isEmpty(), false);
    cout << endl;
    vec.printDebugInfo();
}

PROVIDED_TEST("Add 5 elements and then insert another element at index 3."){
    OurVector vec;
    for (int i = 0; i < 5; i++){
        vec.add(i);
    }

    vec.insert(3, 100);
    EXPECT_EQUAL(vec.size(), 6);
    EXPECT_EQUAL(vec.get(3), 100);

    cout << endl;
    vec.printDebugInfo();
}

PROVIDED_TEST("Add 5 elements and then remove element at index 3."){
    OurVector vec;
    for (int i = 0; i < 5; i++){
        vec.add(i);
    }

    vec.remove(3);
    EXPECT_EQUAL(vec.size(), 4);
    EXPECT_EQUAL(vec.get(3), 4);

    cout << endl;
    vec.printDebugInfo();
}

PROVIDED_TEST("Add 5 elements and then do an insertion at the very beginning and very end of vector."){
    OurVector vec;
    for (int i = 0; i < 5; i++){
        vec.add(i);
    }

    vec.insert(0, 200);
    EXPECT_EQUAL(vec.size(), 6);
    EXPECT_EQUAL(vec.get(0), 200);
    for (int i = 0; i < 5; i++){
        EXPECT_EQUAL(vec.get(i+1), i);
    }
    cout << endl;
    vec.printDebugInfo();

    vec.insert(6, 300);
    EXPECT_EQUAL(vec.size(), 7);
    EXPECT_EQUAL(vec.get(0), 200);
    for (int i = 0; i < 5; i++){
        EXPECT_EQUAL(vec.get(i+1), i);
    }
    EXPECT_EQUAL(vec.get(6), 300);
    vec.printDebugInfo();
}

PROVIDED_TEST("Add 50 elements. Tests expansion (twice)"){
    OurVector vec;
    for (int i = 0; i < 50; i++){
        vec.add(i);
    }

    EXPECT_EQUAL(vec.size(), 50);
    for (int i = 0; i < 50; i++){
        EXPECT_EQUAL(vec.get(i), i);
    }
    EXPECT_EQUAL(vec.isEmpty(), false);
    cout << endl;
    vec.printDebugInfo();
}

