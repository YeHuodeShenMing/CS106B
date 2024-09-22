// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
#include "set.h"
using namespace std;

// 声明函数
int totalVotes(Vector<int> blocks);
Vector<int> standard(Vector<int>& power);
void generateOneBlock(int index, int currUnionVotes, Vector<int>& blocks, Vector<int>& power, int halfVotes);


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    // 初始化result，使其大小与blocks相同，所有元素为0
    Vector<int> result(blocks.size(), 0);
    int halfVotes = totalVotes(blocks) / 2;

    generateOneBlock(0, 0, blocks, result, halfVotes);
    return standard(result);
}

// void listSubsetsHelper(Set<string>& remaining, Set<string>& chosen) {
//     /* Base case: If there are no more decisions to be made, then we've
//      * committed to a specific set.
//      */
//     if (remaining.isEmpty()) {
//         // cout << chosen << endl;
//         generateOneBlock(0,0,chosen,power,halfVotes)

//     }
//     /* Recursive case: Pick an element. Then we either include that element
//      * or exclude that element.
//      */
//     else {
//         string elem = remaining.first();

//         /* Indicate that this element has been considered by removing it
//          * from the set of remaining options. */
//         remaining = remaining - elem;

//         /* Option 1: Do not add element to chosen */
//         listSubsetsHelper(remaining, chosen);

//         /* Option 2: Add element to chosen */
//         chosen = chosen + elem;
//         listSubsetsHelper(remaining, chosen);

//         /* We must explicitly undo the changes we made to the data structures
//          * so that we don't impact the state of the variables in other stack
//          * frames.
//          */
//         chosen = chosen - elem;
//         remaining = remaining + elem;
//     }
// }

void generateOneBlock(int index, int currUnionVotes, Vector<int>& blocks, Vector<int>& power, int halfVotes) {
    // Base case
    if (index == blocks.size()) {
        return;
    }

    /* Option 1: Do not add element to chosen */
    generateOneBlock(index + 1, currUnionVotes, blocks, power, halfVotes);

    /* Option 2: Add element to chosen */
    int targetUnionVotes = currUnionVotes + blocks[index];

    bool isCritical = (targetUnionVotes > halfVotes) && (currUnionVotes < halfVotes);

    if (isCritical) {
        power[index] += 1;
    }

    generateOneBlock(index + 1, targetUnionVotes, blocks, power, halfVotes);

}

int totalVotes(Vector<int> blocks) {
    int count = 0;
    for (int vote : blocks) {
        count += vote;
    }
    return count;
}

Vector<int> standard(Vector<int>& power) {
    int sum = totalVotes(power);

    if (sum == 0) {
        return power;
    }

    for (int i = 0; i < power.size(); i++) {
        power[i] = power[i] * 100 / sum ;
    }
    return power;
}

/* * * * * * Test Cases * * * * * */
STUDENT_TEST("Test standard 1") {
    Vector<int> blocks = {3, 1, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(standard(blocks), expected);
}

STUDENT_TEST("Test standard 2") {
    Vector<int> blocks = {3, 3, 3};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(standard(blocks), expected);
}

STUDENT_TEST("Test standard 3") {
    Vector<int> blocks = {0, 0, 1};
    Vector<int> expected = {0, 0, 100};
    EXPECT_EQUAL(standard(blocks), expected);
}

STUDENT_TEST("Test totalVotes 1") {
    Vector<int> blocks = {50, 49, 1};
    EXPECT_EQUAL(totalVotes(blocks), 100);
}

STUDENT_TEST("Test totalVotes 2") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    EXPECT_EQUAL(totalVotes(blocks), 30);
}



PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


