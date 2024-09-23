#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string already = "";
    EncodingTreeNode* p = tree;
    while(!messageBits.isEmpty()){
        while(!p->isLeaf()){
            if(messageBits.dequeue() == 0){
                p = p->zero;
            }else{
                p = p->one;
            }
        }
        already+=p->ch;
        p = tree;
    }
    return already;
}

// string decodeTextHelper(EncodingTreeNode* tree, Queue<Bit>& messageBits, string& already, EncodingTreeNode* root) {

//     if (tree->isLeaf()) {
//         already += tree->ch;
//         decodeTextHelper(root, messageBits, already, root);
//     }

//     if (!messageBits.isEmpty()) {
//         Bit direct = messageBits.dequeue();

//         if (direct == 0) {
//             decodeTextHelper(tree->zero, messageBits, already, root);
//         }
//         else {
//             decodeTextHelper(tree->one, messageBits, already, root);
//         }
//     }
//     return decodeTextHelper(tree, messageBits, already, tree);
//     return already;
// }


/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    // 存储子节点没存满的curr（父节点）
    Stack<EncodingTreeNode*> unallocatedParentNodes;

    Bit firstByte = treeShape.dequeue();
    if (firstByte == 0 && !treeLeaves.isEmpty()) {
        EncodingTreeNode* root = new EncodingTreeNode(treeLeaves.dequeue());
        return root;
    }
    EncodingTreeNode* root = new EncodingTreeNode(nullptr, nullptr);
    EncodingTreeNode* curr = root;

    unallocatedParentNodes.push(curr);

    while (!treeShape.isEmpty()) {
        // 这里取出的是 下一个该看的元素
        Bit type = treeShape.dequeue();
        if (type == 0 && !treeLeaves.isEmpty()) {
            // 左节点为空
            if (!curr->zero) {
                curr->zero = new EncodingTreeNode(treeLeaves.dequeue());
            }
            // 右节点为空
            else {
                curr->one = new EncodingTreeNode(treeLeaves.dequeue());
                if (!unallocatedParentNodes.isEmpty()) {
                    curr = unallocatedParentNodes.pop();
                }
            }
        }

        // type == 1, 即为内部节点,建立方法为 new EncodingTreeNode(nullptr, nullptr)
        else {
            if(!curr->zero) {
                curr->zero = new EncodingTreeNode(nullptr, nullptr);
                unallocatedParentNodes.push(curr);
                curr = curr->zero;
            }
            else {
                curr->one = new EncodingTreeNode(nullptr, nullptr);
                curr = curr->one;
            }
        }
    }
    return root;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* tree = unflattenTree(data.treeShape, data.treeLeaves);
    string result = decodeText(tree, data.messageBits);
    deallocateTree(tree);
    return result;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    // 建立一个frequency的映射表
    Map<char, int> letterFrequency;
    for (char ch : text) {
        if (letterFrequency.containsKey(ch)) {
            letterFrequency[ch] += 1;
        }
        else {
            letterFrequency.put(ch, 1);
        }
    }

    // 建立一个PQ用来存储节点
    PriorityQueue<pair<EncodingTreeNode*, int>> subtreeList;

    // 把每个不同节点都插入
    pair<EncodingTreeNode*, int> TreeNodeData;

    for (char ch : letterFrequency.keys()) {
        TreeNodeData.first = new EncodingTreeNode(ch);
        TreeNodeData.second = letterFrequency[ch];
        subtreeList.enqueue(TreeNodeData, letterFrequency[ch]);
        // deallocateTree(TreeNodeData.first);
    }

    // while循环 队列元素等于1时结束
    pair<EncodingTreeNode*, int> zero;
    pair<EncodingTreeNode*, int> one;
    EncodingTreeNode* newTmp;
    pair<EncodingTreeNode*, int> newTreeNodeData;

    int newFreq;

    while (subtreeList.size() > 1) {
        // 出队2个元素
        zero = subtreeList.dequeue();
        one = subtreeList.dequeue();

        // 合成一个新节点
        newTmp = new EncodingTreeNode(zero.first, one.first);
        newFreq = zero.second + one.second;

        // 入队,加回PQ

        newTreeNodeData.first = newTmp;
        newTreeNodeData.second = newFreq;
        subtreeList.enqueue(newTreeNodeData, newFreq);
        // deallocateTree(newTmp);
    }
    return subtreeList.dequeue().first;
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    Queue<Bit> res;
    Stack<pair<EncodingTreeNode*, string>> explored;
    EncodingTreeNode* curr = tree;
    Map<char,string> charToseqence;
    string tmp = "";

    pair<EncodingTreeNode*, string> currData;

    currData.first = curr;
    currData.second = tmp;

    explored.push(currData);

    // curr为最右下节点
    while(!explored.isEmpty()) {
        if (curr->zero) {
            tmp += '0';

            if (curr->zero->isLeaf()) {
                charToseqence[curr->zero->getChar()] = tmp;
                // 找到一个Node后重置
                tmp = tmp.substr(0, tmp.size() - 1);
            }
            // 左节点为中间节点
            else {
                currData.first = curr;
                currData.second = tmp.substr(0, tmp.size() - 1);
                explored.push(currData);
                curr = curr->zero;
                continue;
            }
        }
        if (curr->one) {
            tmp += '1';

            if (curr->one->isLeaf()) {
                charToseqence[curr->one->getChar()] = tmp;
                // 找到一个Node后重置
                currData = explored.pop();
                curr = currData.first->one;
                tmp = currData.second + "1";
            }
            // 右节点为中间节点
            else {
                curr = curr->one;
                continue;
            }
        }

        if (curr->isLeaf()) {
            charToseqence[curr->ch] = tmp;
            currData = explored.pop();
            curr = currData.first->one;
            tmp = currData.second + "1";
        }

    }

    Queue<Bit> result;

    for (char c : text) {
        string cStr = charToseqence[c];
        for (char ch : cStr) {
            if (ch == '1') {
                result.enqueue(1);
            }
            else {
                result.enqueue(0);
            }
        }
    }
    return result;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* curr = tree;
    Stack<EncodingTreeNode*> unallocateOne;

    unallocateOne.push(curr);
    // 用来保证不退出while
    unallocateOne.push(curr);
    treeShape.enqueue(1);

    curr = curr->zero;

    while (!unallocateOne.isEmpty()) {
        if (curr->isLeaf()) {
            treeShape.enqueue(0);
            treeLeaves.enqueue(curr->getChar());

            curr = unallocateOne.pop()->one;
        }
        else {
            treeShape.enqueue(1);
            unallocateOne.push(curr);

            curr = curr->zero;
        }
    }

}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    EncodingTreeNode* root = buildHuffmanTree(messageText);

    Queue<Bit> shape;
    Queue<char> leaves;
    flattenTree(root, shape, leaves);

    Queue<Bit> msgBits = encodeText(root, messageText);
    EncodedData result;
    result.treeShape = shape;
    result.treeLeaves = leaves;
    result.messageBits = msgBits;

    deallocateTree(root);

    return  result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */
EncodingTreeNode* createTree() {
    EncodingTreeNode* whitespace = new EncodingTreeNode(' ');
    EncodingTreeNode* H = new EncodingTreeNode('H');
    EncodingTreeNode* I = new EncodingTreeNode('I');
    EncodingTreeNode* A = new EncodingTreeNode('A');
    EncodingTreeNode* Y = new EncodingTreeNode('Y');
    EncodingTreeNode* O = new EncodingTreeNode('O');
    EncodingTreeNode* P = new EncodingTreeNode('P');
    EncodingTreeNode* right4left = new EncodingTreeNode(I, A);
    EncodingTreeNode* right4right = new EncodingTreeNode(Y, O);
    EncodingTreeNode* right3 = new EncodingTreeNode(right4left, right4right);
    EncodingTreeNode* right2 = new EncodingTreeNode(right3, P);

    EncodingTreeNode* left2 = new EncodingTreeNode(whitespace, H);

    EncodingTreeNode* root = new EncodingTreeNode(left2, right2);

    return root;
}

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    // EncodingTreeNode* T = new EncodingTreeNode('T');
    // EncodingTreeNode* R = new EncodingTreeNode('R');
    // EncodingTreeNode* S = new EncodingTreeNode('S');
    // EncodingTreeNode* E = new EncodingTreeNode('E');

    // EncodingTreeNode* tmpTwo = new EncodingTreeNode(R, S);
    // EncodingTreeNode* tmpOne = new EncodingTreeNode(tmpTwo, E);
    // EncodingTreeNode* root = new EncodingTreeNode(T, tmpOne);

    EncodingTreeNode* tree = new EncodingTreeNode(nullptr, nullptr);
    tree->zero = new EncodingTreeNode('T');
    tree->one = new EncodingTreeNode(nullptr, nullptr);
    EncodingTreeNode *one = tree->one;
    one->one = new EncodingTreeNode('E');
    one->zero = new EncodingTreeNode(new EncodingTreeNode('R'), new EncodingTreeNode('S'));
    return tree;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    if (t->zero != nullptr) {
        deallocateTree(t->zero);
    }

    if (t->one != nullptr) {
        deallocateTree(t->one);
    }

    delete t;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (!a && !b) {
        return true;
    }

    if (!a || !b) {
        return false;
    }

    if (a->isLeaf() != b->isLeaf()) {
        return false;
    }

    if (!a->zero && !a->one && !b->zero && !b->one) {
        return a->ch == b->ch;
    }

    return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
}
/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */
// STUDENT_TEST("decodeText") {
//     EncodingTreeNode* tree = createExampleTree();

//     Queue<Bit> message = {1, 0};
//     while (!message.isEmpty()){
//         Bit direct = message.dequeue();
//         cout << direct;
//     }

// }

STUDENT_TEST("unflattenTree") {
    EncodingTreeNode* reference = new EncodingTreeNode('T'); // see diagram above
    Queue<Bit>  treeShape  = { 0 };
    Queue<char> treeLeaves = { 'T' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

STUDENT_TEST("student for decodeText") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 , 1, 1}; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "EE");

    messageBits = { 1, 0, 1, 1, 1, 0 , 1, 0, 0}; // SETR
    EXPECT_EQUAL(decodeText(tree, messageBits), "SETR");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETSSTREETS");

    deallocateTree(tree);
}



STUDENT_TEST("stdent test for helper function") {
    EncodingTreeNode * example = createExampleTree();
    //    deallocateTree(example);

    EncodingTreeNode* tree1 = new EncodingTreeNode('a');
    EncodingTreeNode* tree2 = new EncodingTreeNode('b');
    EncodingTreeNode* nil = nullptr;
    EXPECT(areEqual(tree1, tree1));
    EXPECT(!areEqual(tree1, nil));
    EXPECT(!areEqual(tree1, tree2));
    EXPECT(!areEqual(tree1, example));
    EncodingTreeNode* example2 = createExampleTree();
    EXPECT(areEqual(example, example2));
    EXPECT(!areEqual(example, example->one));
    EXPECT(!areEqual(example->zero, example->one));

    deallocateTree(example);
    deallocateTree(example2);
    deallocateTree(tree1); ;
    deallocateTree(tree2);
}

STUDENT_TEST("test for leak!") {
    EncodingTreeNode* tree = createExampleTree();
    deallocateTree(tree);
}

STUDENT_TEST("test for one leaf node") {
    EncodingTreeNode* singletonTree = new EncodingTreeNode('A');
    EncodingTreeNode* zeroNodeTree = new EncodingTreeNode(NULL);

    EXPECT(!areEqual(singletonTree, zeroNodeTree));

    deallocateTree(singletonTree);
    deallocateTree(zeroNodeTree);
}

STUDENT_TEST("test for second leaf node same with the first one") {
    EncodingTreeNode* singletonTree = new EncodingTreeNode('A');
    EncodingTreeNode* singletonTreeTwo = new EncodingTreeNode('A');

    EncodingTreeNode* singletonTreeThree = new EncodingTreeNode('X');

    EXPECT(areEqual(singletonTree, singletonTreeTwo));
    EXPECT(!areEqual(singletonTree, singletonTreeThree));

    deallocateTree(singletonTree);
    deallocateTree(singletonTreeTwo);
    deallocateTree(singletonTreeThree);
}

STUDENT_TEST("test for example tree") {
    // 创建一个单例树，false
    EncodingTreeNode* singletonTree = new EncodingTreeNode('A');
    EncodingTreeNode* exampleTree = createExampleTree();

    EXPECT(!areEqual(singletonTree, exampleTree));


    deallocateTree(singletonTree);
    deallocateTree(exampleTree);

}

STUDENT_TEST("test for example tree 2") {
    // 创建一个单例树，false
    EncodingTreeNode* exampleTree = createExampleTree();

    // 再次调用example建立树，true

    EncodingTreeNode* anotherExampleTree = createExampleTree();

    EXPECT(areEqual(exampleTree, anotherExampleTree));

    deallocateTree(exampleTree);
    deallocateTree(anotherExampleTree);
}


STUDENT_TEST("test for subtrees") {
    // 子树和exampletree比较，false
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');

    EncodingTreeNode* tmpTwo = new EncodingTreeNode(R, S);
    EncodingTreeNode* tmpOne = new EncodingTreeNode(tmpTwo, E);

    EncodingTreeNode* exampleTree = createExampleTree();

    EXPECT(!areEqual(tmpOne, exampleTree));

    deallocateTree(tmpOne);
    deallocateTree(exampleTree);
}

STUDENT_TEST("encode") {
    EncodingTreeNode* tree = createExampleTree();

    Queue<Bit> messageBits = {1, 1};
    encodeText(tree, decodeText(tree, messageBits));

    Queue<Bit> msgBits = {1,0,1,1,1,0};
    encodeText(tree, decodeText(tree, msgBits));

    Queue<Bit> msgBits2 = {1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1};
    encodeText(tree, decodeText(tree, msgBits2));

    deallocateTree(tree);
}

STUDENT_TEST("flatten") {
    EncodingTreeNode* reference = createExampleTree();

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

STUDENT_TEST("flatten 2") {
    EncodingTreeNode* reference = createTree();

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;

    flattenTree(reference, treeShape, treeLeaves);

    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

STUDENT_TEST("buildHuffman") {
    EncodingTreeNode* result = buildHuffmanTree("KIRK'S DIKDIK");
    deallocateTree(result);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}





