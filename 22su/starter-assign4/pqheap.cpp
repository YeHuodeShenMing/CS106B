#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _numFilled = 0;
    _elements = new DataPoint[_numAllocated];

}

PQHeap::~PQHeap() {
    delete[] _elements;
}

void PQHeap::enqueue(DataPoint elem) {
    if (isFull()) {
        realloc();
    }

    //插入队尾
    _elements[size()] = elem;
    // 记录index
    int newIndex = size();

    // 冒泡newElem
    bubbleUp(newIndex);

    _numFilled += 1;
}


DataPoint PQHeap::peek() const { 
    return _elements[0];
}


DataPoint PQHeap::dequeue() {
    // 将队尾元素先放到0号位，再将size-1
    DataPoint last = _elements[0];
    _elements[0] = _elements[size() - 1];
    _numFilled -= 1;

    // 下沉新元素
    bubbleDown(0);

    return last;
}

//by Ephmeral
bool PQHeap::isFull() const {
    return _numFilled >= _numAllocated;
}

void PQHeap::realloc() {
    if (_numFilled < _numAllocated) {
        return;
    }

    //DataPoint* elem_new = (DataPoint*)malloc(sizeof(DataPoint) * _numAllocated);
    DataPoint* elem_new = new DataPoint[_numAllocated * 2];
    for (int i = 0; i < _numAllocated; ++i) {
        elem_new[i] = _elements[i];
    }
    delete[] _elements;
    _elements = elem_new;
    _numAllocated <<= 1;
}

bool PQHeap::isEmpty() const {
    return _numFilled == 0;
}


int PQHeap::size() const {
    return _numFilled;
}


void PQHeap::clear() {
    _numFilled = 0;
}

bool PQHeap::isValidIndex(int index) const{
    if (index < size()) {
        return true;
    }
    return false;
}

void PQHeap::swap(DataPoint& elem, DataPoint& parentElem) {
    DataPoint tmp = elem;
    elem = parentElem;
    parentElem = tmp;
}

int PQHeap::smaller(int index, int nextIndex) {
    if (_elements[index].priority < _elements[nextIndex].priority) {
        return index;
    }
    return nextIndex;

}


void PQHeap::bubbleDown(int index) {
    int smallIndex;
    int left = getLeftChildIndex(index);
    int right  = getRightChildIndex(index);
     // 2个child都为空，直接返回
    if (left == NONE) {
        return;
    }

    // 左孩子在，右孩子不在
    else if (right == NONE) {
        smallIndex = left;
    }

    // 两孩子都存在
    else {
        smallIndex = smaller(left, right);
    }

    // 较小的数向上冒泡
    if (smaller(index, smallIndex) == smallIndex) {
        swap(_elements[index], _elements[smallIndex]);

        // 更新
        bubbleDown(smallIndex);
    }
}

/*
 * 输入新元素的index后，冒泡至合适位置
 */
void PQHeap::bubbleUp(int index) {
    // priority 小，则swap while循环
    int parentIndex = getParentIndex(index);
    while (smaller(index, parentIndex) == index && parentIndex > NONE) {
        swap(_elements[index], _elements[parentIndex]);

        // 更新
        index = parentIndex;
        parentIndex = getParentIndex(parentIndex);
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * comment about your implementation of the function.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}


void PQHeap::validateInternalState() const {
    /* TODO: Implement this function. */
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than the previous, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority < _elements[getParentIndex(i)].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}


int PQHeap::getParentIndex(int child) const {
    if (child == 0) {
        return NONE;
    }
    return (child - 1) / 2;
}

int PQHeap::getLeftChildIndex(int parent) const {
    int leftChildIndex = parent * 2 + 1;
    if (isValidIndex(leftChildIndex)) {
        return leftChildIndex;
    }
    return NONE;
}

int PQHeap::getRightChildIndex(int parent) const {
    int rightChildIndex = parent * 2 + 2;
    if (isValidIndex(rightChildIndex)) {
        return rightChildIndex;
    }
    return NONE;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */
STUDENT_TEST("test for enqueue") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "X", 10 }, { "Y", 1 }, { "Z", 5 }, { "A", 7 }, { "B", 3 }
    };

    // Enqueue elements and validate internal state after each operation
    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState(); // Ensure heap properties are maintained
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}


/*************STUDENT*************/
STUDENT_TEST("PQHeap basic enqueue and dequeue operations with validateInternalState checks") {
    PQHeap pq;

    // Example data points to enqueue
    Vector<DataPoint> input = {
        { "X", 10 }, { "Y", 1 }, { "Z", 5 }, { "A", 7 }, { "B", 3 }
    };

    // Enqueue elements and validate internal state after each operation
    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState(); // Ensure heap properties are maintained
    }

    EXPECT_EQUAL(pq.size(), input.size());

    // Dequeue elements one by one and validate internal state
    while (!pq.isEmpty()) {
        DataPoint front = pq.dequeue();
        pq.validateInternalState(); // Ensure heap properties are maintained
    }

    EXPECT(pq.isEmpty());
}

STUDENT_TEST("PQHeap enqueue with duplicate priorities and validate internal state") {
    PQHeap pq;

    // Enqueue elements with duplicate priorities
    pq.enqueue({ "Item1", 4 });
    pq.enqueue({ "Item2", 4 });
    pq.enqueue({ "Item3", 4 });
    pq.validateInternalState();

    // Dequeue all elements
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }

    EXPECT(pq.isEmpty());
}

STUDENT_TEST("PQHeap mixed operations (enqueue and dequeue) with validate internal state checks") {
    PQHeap pq;

    // Enqueue some elements
    pq.enqueue({ "Task1", 10 });
    pq.enqueue({ "Task2", 2 });
    pq.validateInternalState();

    // Dequeue one element
    DataPoint front = pq.dequeue();
    EXPECT_EQUAL(front.label, "Task2"); // Task2 has higher priority (lower number)
    pq.validateInternalState();

    // Enqueue more elements
    pq.enqueue({ "Task3", 5 });
    pq.enqueue({ "Task4", 1 });
    pq.validateInternalState();

    // Dequeue all elements
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }

    EXPECT(pq.isEmpty());
}
