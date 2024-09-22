/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h"
#include "soundex.h"
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * WARNING: The provided code is buggy!
 *
 * Add student test cases to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string lettersOnly(string s) {
    string result = "";
    trim(s);
    if (!s.empty()) {
        // result = charToString(s[0]);
        for (int i = 0; i < s.length(); i++) {
            if (isalpha(s[i])) {
                result += s[i];
            }

        }
    }
    return result;
}

string changeLetterToNumber(string& s) {
    s = toUpperCase(s);
    //新建向量， '-'表示未定义字符
    // 大小为128，以覆盖所有可能的ASCII字符
    Vector<char> mapping(128, '-');
    string zero = "AEIOUHWY";
    string one = "BFPV";
    string two = "CGJKQSXZ";
    string three = "DT";
    string four = "L";
    string five = "MN";
    string six = "R";
    for (char z : zero) {
        mapping[z] = '0';
    }
    for (char o : one) {
        mapping[o] = '1';
    }
    for (char t : two) {
        mapping[t] = '2';
    }
    for (char th : three) {
        mapping[th] = '3';
    }
    for (char f : four) {
        mapping[f] = '4';
    }
    for (char fi : five) {
        mapping[fi] = '5';
    }
    for (char s : six) {
        mapping[s] = '6';
    }

    for (int i = 0; i < s.length(); i++) {
        s[i] = mapping[s[i]];
    }
    return s;
}

string deleteAdjcentSameNum(string& s) {
    int slow = 0;
    int fast = 0;
    string newStr = "";
    if (!s.empty()) {
        while (fast < s.length()) {
            if (s[slow] != s[fast]) {
                slow++;
                s[slow] = s[fast];
            }
            fast++;
        }

        for (int i = 0; i < slow + 1; i++) {
            newStr += s[i];
        }
    }
    s = newStr;
    return newStr;

}

string dropZero(string& s) {
    string newStr = "";

    for (char c : s) {
        if (c != '0') {
            newStr += c;
        }
    }
    s = newStr;
    return newStr;
}

string fillZero(string& s){
    int len = s.length();

    if (len < 4){
        while (len < 4) {
            s += '0';
            len++;
        }
        return s;
    }

    else if (len > 4) {
        string newStr = "";
        for (int i = 0; i < 4; i++) {
            newStr += s[i];
        }
        s = newStr;
        return newStr;
    }
    return s;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    string sOnlyLetter = lettersOnly(s);
    string firstLetterInS = toUpperCase(sOnlyLetter);
    changeLetterToNumber(sOnlyLetter);
    deleteAdjcentSameNum(sOnlyLetter);
    sOnlyLetter[0] = firstLetterInS[0];
    dropZero(sOnlyLetter);
    fillZero(sOnlyLetter);
    return sOnlyLetter;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */

    string usrName = getLine("Enter a surname (RETURN to quit): ");
    while (usrName != "")
    {
        string soundexUsrName = soundex(usrName);
        cout << "Soundex code is " << soundexUsrName << endl;
        Vector<string> matchedNames;
        for (string tmpName : allNames) {
            string soundextmpName = soundex(tmpName);
            if (soundexUsrName == soundextmpName) {
                matchedNames.add(tmpName);
            }
        }
        matchedNames.sort();
        cout << "Matches from database: " << matchedNames << endl << endl;
        usrName = getLine("Enter a surname (RETURN to quit): ");
    }

}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!
STUDENT_TEST("QUITE EMPTY : find the bug in lettersOnly") {
    string s1 = "";
    string result1 = lettersOnly(s1);
    EXPECT_EQUAL(result1, "");
}

STUDENT_TEST("QUITE PUCTUATION : find the bug in lettersOnly") {
    string s2 = "!@#$%^";
    string result2 = lettersOnly(s2);
    EXPECT_EQUAL(result2, "");
}

STUDENT_TEST("number & puctuation : find the bug in lettersOnly") {
    string s3 = "!12$6^";
    string result3 = lettersOnly(s3);
    EXPECT_EQUAL(result3, "");
}

STUDENT_TEST("4 : find the bug in lettersOnly") {
    string s3 = "! 12$6^";
    string result3 = lettersOnly(s3);
    EXPECT_EQUAL(result3, "");
}

STUDENT_TEST("letter2num 1") {
    string s = "HELLO";
    EXPECT_EQUAL(changeLetterToNumber(s), "00440");
}

STUDENT_TEST("letter2num 2") {
    string s = "palafawod";
    EXPECT_EQUAL(changeLetterToNumber(s), "104010003");
}

STUDENT_TEST("letter2num 3") {
    string s = "LiuJiachen";
    EXPECT_EQUAL(changeLetterToNumber(s), "4002002005");
}

STUDENT_TEST("deleteSameName 1") {
    string s = "011234";
    EXPECT_EQUAL(deleteAdjcentSameNum(s), "01234");
}

STUDENT_TEST("deleteSameName 2") {
    string s = "01116";
    EXPECT_EQUAL(deleteAdjcentSameNum(s), "016");
}

STUDENT_TEST("deleteSameName 3") {
    string s = "1111111";
    EXPECT_EQUAL(deleteAdjcentSameNum(s), "1");
}

STUDENT_TEST("deleteSameName 4") {
    string s = "";
    EXPECT_EQUAL(deleteAdjcentSameNum(s), "");
}

STUDENT_TEST("deleteSameName 5") {
    string s = "222025";
    EXPECT_EQUAL(deleteAdjcentSameNum(s), "2025");
}

STUDENT_TEST("dropZero 1") {
    string s = "202025";
    EXPECT_EQUAL(dropZero(s), "2225");
}

STUDENT_TEST("dropZero 2") {
    string s = "001";
    EXPECT_EQUAL(dropZero(s), "1");
}

STUDENT_TEST("dropZero 3") {
    string s = "100";
    EXPECT_EQUAL(dropZero(s), "1");
}

STUDENT_TEST("fillZero 1") {
    string s = "100";
    EXPECT_EQUAL(fillZero(s), "1000");
}

STUDENT_TEST("fillZero 2") {
    string s = "1002";
    EXPECT_EQUAL(fillZero(s), "1002");
}

STUDENT_TEST("fillZero 3") {
    string s = "";
    EXPECT_EQUAL(fillZero(s), "0000");
}

STUDENT_TEST("STEP B : soundex 1") {
    string s = "a3ja2&^ 206";
    EXPECT_EQUAL(soundex(s), "A200");
}

STUDENT_TEST("STEP B : soundex 2") {
    string s = "#%*KaD45ASDFr";
    EXPECT_EQUAL(soundex(s), "K323");
}

STUDENT_TEST("STEP C : soundex 3") {
    string s = "Curie";
    EXPECT_EQUAL(soundex(s), "C600");
}

STUDENT_TEST("STEP C : soundex 4") {
    string s = "O'Conner";
    EXPECT_EQUAL(soundex(s), "O256");
}

STUDENT_TEST(" soundex ") {
    string s = "Zelenski";
    EXPECT_EQUAL(soundex(s), "Z452");
}
/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}
