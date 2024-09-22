// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    Vector<GridLocation> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (GridLocation direct : directions) {
        int newRow = direct.row + cur.row;
        int newCol = direct.col + cur.col;
        if (maze.inBounds(newRow, newCol) && maze[newRow][newCol] == true){
            neighbors.add(GridLocation(newRow, newCol));
        }
    }

    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
    GridLocation mazeEnter = {0, 0};
    Set<GridLocation> alreadyExplore;

    while (path.size() > 1) {
        GridLocation curLocation = path.pop();
        alreadyExplore.add(curLocation);
        GridLocation nextLocation = path.peek();
        if (!generateValidMoves(maze, curLocation).contains(nextLocation))
        {
            error("Path does not end at maze exit");
        }
        if (alreadyExplore.contains(nextLocation)) {
            error("Path does not end at maze exit");
        }
    }

    if (path.peek() != mazeEnter) {
        error("Path does not end at maze exit");
    }
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;


    Queue<Stack<GridLocation>> allPaths;
    Stack<GridLocation> initialPath;
    Set<GridLocation> exploredLocation;

    GridLocation mazeEnter = {0, 0};
    exploredLocation.add(mazeEnter);
    GridLocation mazeExit = {maze.numRows() - 1, maze.numCols() - 1};

    initialPath.push(mazeEnter);
    allPaths.enqueue(initialPath);

    if (!maze[mazeEnter]) {  // 如果入口无法通行，直接返回空路径
        return {};
    }

    while(!allPaths.isEmpty()) {
        Stack<GridLocation> currPath = allPaths.dequeue();

        MazeGraphics::highlightPath(currPath, "blue");

        if (currPath.peek() == mazeExit) {
            MazeGraphics::highlightPath(currPath, "green");
            return currPath;
        }

        else {
            // Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur)
            Set<GridLocation> validNeighbors = generateValidMoves(maze, currPath.peek());

            for (GridLocation validNeighbor : validNeighbors) {
                if (!exploredLocation.contains(validNeighbor)) {
                    exploredLocation.add(validNeighbor);
                    Stack<GridLocation> tempValidPath = currPath;
                    tempValidPath.push(validNeighbor);
                    allPaths.enqueue(tempValidPath);
                }
            }

        }
    }
    // return false;

    /************************/

    return {};
}


// string countingSort(string s) {
//     Map<char, int> freqMap;
//     for (char ch : s) {
//         freqMap[ch] += 1;
//     }

//     string newString ;
//     for (char ch = 'a'; ch < 'z' + 1; ch++) {
//         while (freqMap[ch] > 0) {
//             newString += ch;
//             freqMap[ch]--;
//         }
//     }
//     return newString;
// }







/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

// STUDENT_TEST("1") {
//     string str = "acuupied";
//     string expected = "acdeipuu";

//     EXPECT_EQUAL(countingSort(str), expected);
// }

STUDENT_TEST("next to right way") {
    Grid<bool> maze = {{true, true, true},
                       {false, false, false},
                       {true, true, true}};
    GridLocation edge = {2, 1};
    Set<GridLocation> expected = {{2, 0}, {2, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, edge), expected);
}


STUDENT_TEST("no right way") {
    Grid<bool> maze = {{true, true, true},
                       {false, false, false},
                       {true, true, true}};
    GridLocation edge = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, edge), expected);
}

STUDENT_TEST("next to right way") {
    Grid<bool> maze = {{true, false},
                       {false, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

STUDENT_TEST("validatePath 1") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/17x37.maze", maze);
    readSolutionFile("res/17x37.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath 2") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath 3") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {true, true, true}};;
    Stack<GridLocation> soln = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2,2}};

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath 4") {
    Grid<bool> maze = {{true, true, true},
                       {true, false, true},
                       {true, true, true}};;
    Stack<GridLocation> soln = {{0, 0}, {0, 2}, {1, 2}, {2,2}};

    EXPECT_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("wrong maze 1") {
    Grid<bool> maze;
    EXPECT_ERROR(readMazeFile("res/2x3_wrong.maze", maze));
}

STUDENT_TEST("wrong maze 2") {
    Grid<bool> maze;
    EXPECT_ERROR(readMazeFile("res/13x39_wrong.maze", maze));
}

STUDENT_TEST("wrong maze 3") {
    Grid<bool> maze;
    EXPECT_ERROR(readMazeFile("res/5x7_wrong.maze", maze));
}

STUDENT_TEST("solveMaze on file 13x39") {
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x25") {
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 25x15") {
    Grid<bool> maze;
    readMazeFile("res/25x15.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
/*********************************/
// TODO: add your test cases ABOVE here
/*********************************/


PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


