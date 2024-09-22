//
// Created by LIU JIACHEN on 2024/9/15.
//

#include <stdio.h>
#include <stdlib.h>

void printBytes() {
    
}

int main() {
    printBytes();
}

//typedef int ElementType;
//typedef struct Node *PtrToNode;
//struct Node {
//    ElementType Data;
//    PtrToNode   Next;
//};
//typedef PtrToNode List;
//
//List Read(); /* 细节在此不表 */
//void Print( List L ); /* 细节在此不表 */
//
//List Reverse( List L );
//
//int main()
//{
//    List L1, L2;
//    L1 = Read();
//    L2 = Reverse(L1);
//    Print(L1);
//    Print(L2);
//    return 0;
//}
//
///* 你的代码将被嵌在这里 */
//List Reverse (List L) {
//    List tmp = L->Next;
//    List prev = NULL;
//
//    while (tmp != NULL) {
//        L->Next = prev;
//        prev = L;
//        L = tmp;
//        tmp = tmp->Next;
//    }
//
//    return prev;
//
//}
//
//
//List Read()
//{
//    List head = NULL;
//    List current;
//    List prev = NULL;
//    int len = 0;
//    scanf("%d", &len);
//    if (len == 0)   return NULL;
//
//    while (len--)
//    {
//        current = (List)malloc(sizeof(struct Node));
//        if (head == NULL)
//            head = current;
//        else
//            prev->Next = current;
//        current->Next = NULL;
//        scanf("%d", &current->Data);
//        prev = current;
//    }
//    return head;
//}
//
//void Print(List L)
//{
//    List p = L;
//    List s = L;
//    List temp;
//    if (p == NULL)
//        printf("NULL");
//    else
//        printf("\n");
//    while (p!=NULL) {
//        printf("%d ", p->Data);
//        p = p->Next;
//    }
//}
