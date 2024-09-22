#include "pet.h"
#include "string"
#include "iostream"
#include "SimpleTest.h"

using namespace std;

void petPointer() {
    string pet = "cat";
    string* petPointer = addressof(pet);
    cout << petPointer << endl;
}


STUDENT_TEST("pointer") {
    petPointer();
}
