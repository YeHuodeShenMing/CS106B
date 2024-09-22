/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    if (str.length() < 4) {
        return 0;
    }
    else {
        if (str.length() >= 4) {
            return str.length() - 3;
        }
    }
    return 0;
}

int countPoints(Set<string>& strs) {
    int totalCount = 0;
    for (string str : strs ){
        totalCount += points(str);
    }
    return totalCount;
}

// 看从单个cube出发的是否能继续探索，碰见符合的单词就加入set<string>，返回的是该格子出发探索到的单词集合
Set<string> generateFromOneCube(Grid<char>& board, Lexicon& lex, Grid<bool>& alreadyExplore, GridLocation curr, string soFar, Set<string>& validWords) {
    Vector<GridLocation> directs = {
        {0, 1},  // 右
        {0, -1}, // 左
        {1, 0},  // 下
        {-1, 0}, // 上
        {1, 1},  // 右下对角线
        {1, -1}, // 左下对角线
        {-1, 1}, // 右上对角线
        {-1, -1} // 左上对角线
    };

    // 初始化curr
    // 把alreadyExplore格子改成false，表明，无法再用。
    alreadyExplore[curr.row][curr.col] = false;
    // 把当前格子加进字符串
    soFar += charToString(board[curr.row][curr.col]);

    if (lex.containsPrefix(soFar)) {
        if (lex.contains(soFar) && soFar.size() >= 4) {
            validWords.add(soFar);
        }

        for (GridLocation direct : directs) {
            int newRow = curr.row + direct.row;
            int newCol = curr.col + direct.col;

            if (board.inBounds(newRow, newCol) && alreadyExplore[newRow][newCol] == true) {
                generateFromOneCube(board, lex, alreadyExplore, {newRow, newCol}, soFar, validWords);
            }
        }
    }

    alreadyExplore[curr.row][curr.col] = true;
    soFar = soFar.substr(0, soFar.size() - 1);

    return validWords;
}


// 这一版先测试返回的set
Set<string> validBoard(Grid<char>& board, Lexicon& lex) {
    Set<string> validWords;
    for (int i = 0; i < board.numRows(); i++) {
        for (int j = 0; j < board.numCols(); j++) {
            Grid<bool> explored = { {true, true, true, true},
                                   {true, true, true, true},
                                   {true, true, true, true},
                                   {true, true, true, true}};

            //遍历各个cube
            generateFromOneCube(board, lex, explored, {i, j}, "", validWords);
        }
    }

    return validWords;
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    Set<string> words = validBoard(board, lex);

    return countPoints(words);
}


/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

STUDENT_TEST("test for one cube") {
    Grid<bool> explored = { {true, true, true, true},
                           {true, true, true, true},
                           {true, true, true, true},
                           {true, true, true, true}};
    // Grid<char>& board, Lexicon& lex, Grid<bool>& alreadyExplore, GridLocation curr, string soFar
    Grid<char> board = {
        {'J', '*', '*', '*'},
        {'*', 'U', '*', ' '},
        {'*', '*', 'M', 'P'},
        {'*', '*', '*', '*'}
    };
    Set<string> validWords;
    EXPECT_EQUAL(generateFromOneCube(board, sharedLexicon(), explored, {0, 0}, "", validWords), {"JUMP"});
}



STUDENT_TEST("test for generateFromOneCube") {
    Grid<bool> explored = { {true, true, true, true},
                           {true, true, true, true},
                           {true, true, true, true},
                           {true, true, true, true}};
    // Grid<char>& board, Lexicon& lex, Grid<bool>& alreadyExplore, GridLocation curr, string soFar
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> validWords;
    EXPECT_EQUAL(generateFromOneCube(board, sharedLexicon(), explored, {2, 3}, "", validWords), {"HORIZON"});
    validWords = {};
    EXPECT_EQUAL(generateFromOneCube(board, sharedLexicon(), explored, {1, 3}, "", validWords), {"OHIA", "ORZO"});
    validWords = {};
    EXPECT_EQUAL(generateFromOneCube(board, sharedLexicon(), explored, {3, 2}, "", validWords), {"HAJI"});
    validWords = {};
    EXPECT_EQUAL(generateFromOneCube(board, sharedLexicon(), explored, {2, 1}, "", validWords), {"JOHN"});

}

STUDENT_TEST("test for words from a board 1") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> expectedWords = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};


    EXPECT_EQUAL(validBoard(board, sharedLexicon()), expectedWords);
}

STUDENT_TEST("test for words from a board 2") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> expectedWords = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};


    EXPECT_EQUAL(validBoard(board, sharedLexicon()), expectedWords);
}

/**************************/

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
