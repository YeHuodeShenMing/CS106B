#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

void quickSortHelper(ListNode* head, ListNode* tail) {
    if (head == tail || head->next == tail) {
        return;
    }
    ListNode* slow = head;
    ListNode* fast = head;

    int pivot = head->data;

    while(fast != tail) {
        if (fast->data < pivot) {
            slow = slow->next;

            // 将较小的值（fast所指）与slow交换
            int tmp = slow->data;
            slow->data = fast->data;
            fast->data = tmp;
        }

        fast = fast->next;
    }

    //将隔板换到中间，形成隔板左边<pivot, 右边>pivot
    head->data = slow->data;
    slow->data = pivot;
    quickSortHelper(head,slow->next);
    quickSortHelper(slow->next, tail);

}

void quickSort(ListNode*& front) {
    quickSortHelper(front, nullptr);

}


/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    ListNode* cur = front;
    while (cur != nullptr) {
        ListNode* nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* sentinal = new ListNode(values[0], nullptr); // 头指针

    ListNode* curr = sentinal;

    for (int value : values.subList(1)) {
        ListNode* newNode = new ListNode(value, nullptr);

        curr->next = newNode;
        curr = curr->next;
    }
    return sentinal;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    // 先比较长度
    ListNode* curr = front;
    int linkSize = 0;

    while (curr != nullptr) {
        linkSize += 1;
        curr = curr->next;
    }

    if (linkSize != v.size()) {
        return false;
    }

    curr = front;
    linkSize = 0;

    while (curr != nullptr && linkSize < v.size()) {
        if (curr->data != v[linkSize]) {
            return false;
        }

        linkSize += 1;
        curr = curr->next;
    }

    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {7,6,1,5, 2, 3, 4,10};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}
































// ListNode* vir = front;
// if(vir == nullptr) return;
// if(vir->next ==  nullptr) return;
// ListNode* left = nullptr;
// ListNode* right = nullptr;
// ListNode* pleft = left,*pright = right;
// ListNode* p = vir->next;
// vir->next = nullptr;
// while(p){
//     if(p->data<vir->data){
//         if(!left){
//             left = p;
//             pleft = left;
//             p = p->next;
//         }
//         else{
//             pleft->next = p;
//             pleft = p;
//             p = p->next;
//         }
//     }
//     else{
//         if(!right){
//             right = p;
//             pright = right;
//             p = p->next;
//         }else{
//             pright->next = p;
//             pright = p;
//             p = p->next;
//         }
//     }
// }
// if(pleft) pleft->next = nullptr;
// if(pright) pright->next = nullptr;
// quickSort(left);
// quickSort(right);
// if(pleft){
//     while(pleft->next){
//         pleft = pleft->next;
//     }
//     pleft->next = vir;
// }
// if(right) vir->next = right;
// if(left) front = left;
// else front = vir;
