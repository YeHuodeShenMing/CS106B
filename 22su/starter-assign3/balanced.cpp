/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    return operatorsFromHelper(str, "");
}

string operatorsFromHelper(string str, string soFar) {
    Set<char> validChar = {'(', ')', '[', ']', '{', '}'};
    char first = str[0];
    if (validChar.contains(first)) {
        soFar += first;
    }
    if (str == "") {
        return soFar;
    }
    return operatorsFromHelper(str.substr(1), soFar);
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    Set<char> validChar = {'(', ')', '[', ']', '{', '}'};
    for (char s : ops) {
        if (!validChar.contains(s)) {
           return false;
        }
    }
    if (ops == "") {
        return true;
    }
    else if (ops == "()" || ops == "[]" || ops == "{}") {
        return true;
    }

    else {
        int firstRound = ops.find("()");
        int firstSquare = ops.find("[]");
        int firstCurly = ops.find("{}");

        if (firstRound == string::npos) {
            firstRound = ops.size() - 1;
        }
        if (firstSquare == string::npos) {
            firstSquare = ops.size() - 1;
        }
        if (firstCurly == string::npos) {
            firstCurly = ops.size() - 1;
        }

        int firstPairIndex = min(firstRound, min(firstSquare, firstCurly));
        // 判断配对
        if (firstPairIndex == ops.size() - 1) {
            return false;
        }
        return operatorsAreMatched(ops.substr(0, firstPairIndex) + ops.substr(firstPairIndex + 2));
    }
    /*
     * {([])}
     * first = 2
     *
    */
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */
STUDENT_TEST("operatorsFrom on parentheses example") {
    EXPECT_EQUAL(operatorsFrom("function(a, b)"), "()");
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom("arr[5] + arr[10]"), "[][]");
    EXPECT_EQUAL(operatorsFrom("{key: value}"), "{}");
    EXPECT_EQUAL(operatorsFrom("map[key] = {1, 2, 3}"), "[]{}");
    EXPECT_EQUAL(operatorsFrom("(a[0] + b{2}) * c[3]"), "([]{})[]");
}

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("{vec[3]}"), "{[]}");
    EXPECT_EQUAL(operatorsFrom("{()vec[3]}"), "{()[]}");
    EXPECT_EQUAL(operatorsFrom("{vec[(3)]}"), "{[()]}");
}

STUDENT_TEST("operatorsAreMatched on simple example 1") {
    EXPECT(!operatorsAreMatched("{vec[3]}"));
    EXPECT(!operatorsAreMatched("{()vec[3]}"));
    EXPECT(operatorsAreMatched("{[()]}"));
}

STUDENT_TEST("operatorsAreMatched on simple example 2") {
    EXPECT(!operatorsAreMatched("{[()}"));
}

STUDENT_TEST("operatorsAreMatched on simple example 3") {
    EXPECT(operatorsAreMatched("{()[()]}"));
}

STUDENT_TEST("operatorsAreMatched on simple example 4") {
    EXPECT(operatorsAreMatched("{[]()[]}"));
}


PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
