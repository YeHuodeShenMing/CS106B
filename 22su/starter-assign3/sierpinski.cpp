/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int cnt = 0;

int drawSierpinskiTriangleHelper(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order < 0) {
        error("order must be a positive num!!!");
    }
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        cnt ++;
    }
    else {
        GPoint leftMiddle = {(one.x + two.x) / 2, (one.y + two.y) / 2};
        GPoint rightMiddle = {(one.x + three.x) / 2, (one.y + three.y) / 2};
        GPoint bottomMiddle = {(two.x + three.x) / 2, (two.y + three.y) / 2};

        drawSierpinskiTriangle(window, one, leftMiddle, rightMiddle, order - 1);
        drawSierpinskiTriangle(window, leftMiddle, two, bottomMiddle, order - 1);
        drawSierpinskiTriangle(window, rightMiddle, bottomMiddle, three, order - 1);
    }
    return cnt;
}

int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* 1
     2   3
     */
    int res = 0;
    if (order < 0) {
        error("order must be a positive num!!!");
    }
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    }
    else {
        GPoint leftMiddle = {(one.x + two.x) / 2, (one.y + two.y) / 2};
        GPoint rightMiddle = {(one.x + three.x) / 2, (one.y + three.y) / 2};
        GPoint bottomMiddle = {(two.x + three.x) / 2, (two.y + three.y) / 2};

        res+=drawSierpinskiTriangle(window, one, leftMiddle, rightMiddle, order - 1);
        res+=drawSierpinskiTriangle(window, leftMiddle, two, bottomMiddle, order - 1);
        res+=drawSierpinskiTriangle(window, rightMiddle, bottomMiddle, three, order - 1);
    }
    return res;
    // return drawSierpinskiTriangleHelper(window, one, two, three, order);
}

// int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
//     /* 1
//      2   3
//      */
//     return drawSierpinskiTriangleHelper(window, one, two, three, order);
// }

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}
