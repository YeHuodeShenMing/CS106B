#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "recursion.h"

#include "string"
#include "simpio.h"
using namespace std;

/*
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading
 */


// 原始
int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}


// int main() {
//     int numValues = getInteger("How Many Lines? ");
//     string* arr = new string[numValues];

//     for (int i = 0; i < numValues; i++) {
//         arr[i] = getLine("Enter a string: ");
//     }

//     for(int i = 0; i < numValues; i++ ){
//         cout << i << " : " << arr[i] << endl;
//     }
//     delete[] arr;
//     return 0;
// }

// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    string str;
    int i = 0;
    bool b;
    b = isBalanced(str);
    str = operatorsFrom(str);
    b = operatorsAreMatched(str);
    GPoint pt;
    GWindow win;
    drawSierpinskiTriangle(win, pt, pt, pt, i);
    Queue<int> q;
    Vector<Queue<int>> v;
    q = binaryMerge(q, q);
    q = naiveMultiMerge(v);
    q = recMultiMerge(v);
}
