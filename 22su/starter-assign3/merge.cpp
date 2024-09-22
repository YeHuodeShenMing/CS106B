/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

void orderCheck(Queue<int> a, Queue<int> b) {
    while (a.size() > 1) {
        if (a.dequeue() > a.peek()) {
            error("a not ordered!!!");
        }
    }
    while (b.size() > 1) {
        if (b.dequeue() > b.peek()) {
            error("b not ordered!!!");
        }
    }
}

Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    orderCheck(a, b);

    while (!a.isEmpty() && !b.isEmpty()){
        if (a.peek() <= b.peek()) {
            result.enqueue(a.dequeue());
        }
        else {
            result.enqueue(b.dequeue());
        }
    }
    while (!a.isEmpty()) {
        result.enqueue(a.dequeue());
    }
    while (!b.isEmpty()) {
        result.enqueue(b.dequeue());
    }

    /* TODO: Implement this function. */
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    Queue<int> empty = {};

    if (all.size() == 1) {
        return binaryMerge(all[0], empty);
    }
    if (all.size() == 2) {
        return binaryMerge(all[0], all[1]);
    }

    Vector<Queue<int>> left = all.subList(0, all.size() / 2);
    Vector<Queue<int>> right = all.subList(all.size() / 2);

    Queue<int> leftRes = recMultiMerge(left);
    Queue<int> rightRes = recMultiMerge(right);
    result = binaryMerge(leftRes, rightRes);

    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

STUDENT_TEST("NOT ORDERED 1") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 4, 3};
    Queue<int> c = {2, 6, 5};
    Queue<int> d = {1, 4, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(c, d));
}

STUDENT_TEST("binaryMerge, two short sequences 2") {
    Queue<int> a = {2, 4, 5, 7};
    Queue<int> b = {1, 3, 8};
    Queue<int> expected = {1, 2, 3, 4, 5, 7, 8};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("naiveMultiMerge, multiple sequences with duplicates and empty sequences") {
    Vector<Queue<int>> all = {{2, 4, 6, 8, 10},
        {1, 3, 5, 7, 9},
        {},
        {2, 4, 6},
        {5, 15},
        {0}
    };
    Queue<int> expected = {0, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7, 8, 9, 10, 15};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, single sequence with multiple sequences") {
    Vector<Queue<int>> all = {{5, 10, 15, 20},
        {1, 2, 3},
        {8, 16},
        {},
        {6, 12}
    };
    Queue<int> expected = {1, 2, 3, 5, 6, 8, 10, 12, 15, 16, 20};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("recMultiMerge") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
        {1, 5, 9, 9, 12},
        {5},
        {},
        {-5, -5},
        {3402}
    };

    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

/*****************************/

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
