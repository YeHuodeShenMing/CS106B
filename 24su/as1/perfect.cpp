/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h"
#include "perfect.h"
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

long smarterSum(long n) {
    long total = 0;
    long pairQuiotent = 0;
    for (long divisor = 1; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            if (divisor != 1) {
                pairQuiotent = n / divisor;
            }
            if (divisor != pairQuiotent) {
                total += pairQuiotent;
            }
            total += divisor;
        }
    }
    return total;
}

bool isPerfectSmarter(long n) {
    return (n != 0) && (n != 1) && (n == smarterSum(n));
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Find a smart NUMBER : " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush;
        }
    }
    cout << endl << "Done searching up to " << stop <<"." <<endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {
    int i = 0;
    long k = 1;
    long m = 2;
    long res = 1;
    while (i < n) {
        m = pow(2, k);
        res = (m - 1) * pow(2, (k - 1));
        if (isPerfectSmarter(res)) {
            i++;
        }
        k++;
    }
    return res;
}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!

STUDENT_TEST("Time trial of findPerfects on input size 2000") {
    TIME_OPERATION(2000, findPerfects(2000));
}

STUDENT_TEST("Time trial of findPerfects on input size 4000") {
    TIME_OPERATION(4000, findPerfects(4000));
}

STUDENT_TEST("Time trial of findPerfects on input size 8000") {
    TIME_OPERATION(8000, findPerfects(8000));
}

STUDENT_TEST("Negative number is not a Right choice.") {
    EXPECT_EQUAL(false, isPerfect(-100));
    EXPECT_EQUAL(false, isPerfect(-1000000));
    EXPECT_EQUAL(false, isPerfect(-1000000000));
}

STUDENT_TEST("Test for smarterSum") {
    EXPECT_EQUAL(smarterSum(25),divisorSum(25));
    EXPECT_EQUAL(smarterSum(31),divisorSum(31));
    EXPECT_EQUAL(smarterSum(36),divisorSum(36));
    EXPECT_EQUAL(smarterSum(48),divisorSum(48));
    EXPECT_EQUAL(smarterSum(216),divisorSum(216));
}

STUDENT_TEST("Test for isPerfectSmarter") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
    EXPECT(!isPerfectSmarter(25));
    EXPECT(isPerfectSmarter(6));
    EXPECT(isPerfectSmarter(28));
    EXPECT(isPerfectSmarter(496));
    EXPECT(isPerfectSmarter(8128));
}


STUDENT_TEST("Test for isPerfectSmarter") {
    TIME_OPERATION(1000, findPerfectsSmarter(1000));
    TIME_OPERATION(2000, findPerfectsSmarter(2000));
    TIME_OPERATION(4000, findPerfectsSmarter(4000));
    TIME_OPERATION(8000, findPerfectsSmarter(8000));
    TIME_OPERATION(16000, findPerfectsSmarter(16000));
    // TIME_OPERATION(180000, findPerfectsSmarter(180000));
}

STUDENT_TEST("Test for the EXACT PerfectNumber") {
    EXPECT_EQUAL(6, findNthPerfectEuclid(1));
    EXPECT_EQUAL(28, findNthPerfectEuclid(2));
    EXPECT_EQUAL(496, findNthPerfectEuclid(3));
    EXPECT_EQUAL(8128, findNthPerfectEuclid(4));
    EXPECT_EQUAL(33550336, findNthPerfectEuclid(5));

}

STUDENT_TEST("Test for whether it's PerfectNumber") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(1)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(3)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(4)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(5)));
    // EXPECT(isPerfectSmarter(findNthPerfectEuclid(6)));
    // EXPECT(isPerfectSmarter(findNthPerfectEuclid(7)));
}

/*
 * Here is sample test demonstrating how to use a loop to set the input sizes
 * for a sequence of time trials.
 */
STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {
   int smallest = 1000, largest = 8000;
   for (int size = smallest; size <= largest; size *= 2) {
       TIME_OPERATION(size, findPerfects(size));
   }
}


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
    TIME_OPERATION(2000, findPerfects(2000));
    TIME_OPERATION(4000, findPerfects(4000));
    TIME_OPERATION(8000, findPerfects(8000));
    TIME_OPERATION(16000, findPerfects(16000));
    // TIME_OPERATION(180000, findPerfects(180000));
}


