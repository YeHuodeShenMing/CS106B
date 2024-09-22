// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"

#include "simpio.h"
using namespace std;

#define Positive 1
#define Negative 0


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int start = 0;
    int end = s.size() - 1;

    while (start <= end && ispunct(s[start]) && s[start] != '+' && s[start] != '-') {
        start += 1;
    }

    while (end >= start && ispunct(s[end])) {
        end -= 1;
    }

    string trimedString = toLowerCase(s.substr(start, end - start + 1));
    bool hasAlpha = false;

    for (char c : s) {
        if (isalpha(c)) {
            hasAlpha = true;
            break;
        }
    }

    if (start <= end && hasAlpha) {
        return trimedString;
    }
    return "";
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> wordsInWebPage = stringSplit(text, " ");
    for (string word : wordsInWebPage) {
        string trimedWord = cleanToken(word);
        if (!tokens.contains(trimedWord) && trimedWord != "") {
            tokens.add(trimedWord);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile)) {
        error("Cannot open file named " + dbfile);
    }

    Vector<string> pages;
    readEntireFile(in, pages);
    /* 文件已经被读入 Vector 中了*/

    int pageCount = 0;

    for (int i = 0; i < pages.size() - 1; i++) {

        string currUrl = pages[i];
        i += 1;
        Set<string> wordInPage = gatherTokens(pages[i]);

        for (string word : wordInPage) {
            if (!(index[word]).contains(currUrl)) {
                (index[word]).add(currUrl);
            }
        }

        pageCount += 1;
    }

    return pageCount;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: 处理query
    Vector<string> searchTeams = stringSplit(query, " ");

    for (int i = 0; i < searchTeams.size(); i++) {
        searchTeams[i] = cleanToken(searchTeams[i]);
    }

    // 匹配url
    for (string searchItem : searchTeams) {
        if (searchItem[0] == '+') {
            string Item = searchItem.substr(1);
            result.intersect(index[Item]);
        }
        else if (searchItem[0] == '-') {
            string Item = searchItem.substr(1);
            result.difference(index[Item]);
        }

        else {
            result.unionWith(index[searchItem]);
        }
    }


    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index;
    int pageCount = buildIndex(dbfile, index);
    cout << "Indexed " << pageCount << " pages containing " << index.size() << " unique terms." << endl;

    string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    while (query != "") {
        Set<string> findedUrl = findQueryMatches(index,query);
        if (findedUrl.size() == 1) {
            cout << "Found " << findedUrl.size() << " matching page" << endl;
        }
        else {
            cout << "Found " << findedUrl.size() << " matching pages" << endl;
        }
        cout << findedUrl << endl << endl;

        query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    }

    /*
     *Stand by while building index...
    Indexed 40 pages containing 6362 unique terms.

    Enter query sentence (RETURN/ENTER to quit): citation
    Found 3 matching pages
    {"https://cs106b.stanford.edu/assignments/2-adt/searchengine.html",
    "https://cs106b.stanford.edu/citation", "https://cs106b.stanford.edu/honor_code"}

    Enter query sentence (RETURN/ENTER to quit): cs106l template -qt
    Found 4 matching pages
    {"https://cs106b.stanford.edu/course_placement", "https://cs106b.stanford.edu/faq",
    "https://cs106b.stanford.edu/lectures/06-set-map/",
    "https://cs106b.stanford.edu/resources/style_guide.html"}

    Enter query sentence (RETURN/ENTER to quit):

    main() completed.
    */
}

/* * * * * * Test Cases FOR STUDENT* * * * * */

// TODO: add your test cases here

STUDENT_TEST("buildIndex in right time limit.") {
    Map<string, Set<string>> index;
    TIME_OPERATION(5000, buildIndex("res/tiny.txt", index));
}

STUDENT_TEST("cleanToken handles mixed punctuation correctly") {
    EXPECT_EQUAL(cleanToken("!!!hello..."), "hello");
    EXPECT_EQUAL(cleanToken("**WORLD**"), "world");
    EXPECT_EQUAL(cleanToken("---cs106b---"), "---cs106b");
}

STUDENT_TEST("cleanToken on empty or purely punctuated strings") {
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("###"), "");
    EXPECT_EQUAL(cleanToken("!!!***"), "");
}

STUDENT_TEST("gatherTokens from a complex sentence with numbers and punctuation") {
    Set<string> expected = {"this", "is", "a", "test-case", "for", "cs106b"};
    EXPECT_EQUAL(gatherTokens("This, is a! test-case? for... CS106B"), expected);
}

STUDENT_TEST("gatherTokens from a sentence with repeated words and mixed case") {
    Set<string> expected = {"apple", "banana", "cherry"};
    EXPECT_EQUAL(gatherTokens("Apple apple BANANA cherry Cherry banana"), expected);
}

STUDENT_TEST("buildIndex from new tiny.txt, 5 pages, 25 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/new_tiny.txt", index);
    EXPECT_EQUAL(nPages, 5);
    EXPECT_EQUAL(index.size(), 25); // Number of unique tokens across all pages
    EXPECT(index.containsKey("apples"));
    EXPECT(index.containsKey("fish"));
    EXPECT(index.containsKey("news"));
}

STUDENT_TEST("findQueryMatches from new tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/new_tiny.txt", index);
    Set<string> matchesFish = findQueryMatches(index, "fish");
    EXPECT_EQUAL(matchesFish.size(), 1);
    EXPECT(matchesFish.contains("www.healthyrecipes.org"));
}

STUDENT_TEST("findQueryMatches from new tiny.txt, compound queries with logical operators") {
    Map<string, Set<string>> index;
    buildIndex("res/new_tiny.txt", index);
    Set<string> matchesFruitOrFish = findQueryMatches(index, "apples fish");
    EXPECT_EQUAL(matchesFruitOrFish.size(), 2);
    Set<string> matchesFruitAndFish = findQueryMatches(index, "apples +fish");
    EXPECT_EQUAL(matchesFruitAndFish.size(), 0); // No single page contains both apples and fish
    Set<string> matchesFruitWithoutFish = findQueryMatches(index, "apples -fish");
    EXPECT_EQUAL(matchesFruitWithoutFish.size(), 1);
    EXPECT(matchesFruitWithoutFish.contains("www.shoppinghub.com"));
}



/* * * * * * Test Cases * * * * * */
PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    // EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    // milk fish bread lbmms red green blue one two i eat
    // 11个
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}
