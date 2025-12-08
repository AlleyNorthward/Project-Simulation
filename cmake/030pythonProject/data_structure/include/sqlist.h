#ifndef _SQLIST_H
#define _SQLIST_H

#define TRUE_                        1
#define FALSE_                       0
#define OK_                          1
#define ERROR_                       0
#define INFEASIBLE_                 -1
#define OVERFLOW_                   -2
// #define NULL                        0

#define LIST_INIT_SIZE              100

typedef int ElemType;
typedef int Status;

typedef struct{
    ElemType* base;
    int capacity;
    int size;
}SqList;

Status InitList_Sq(SqList* L);
Status DestroyList_Sq(SqList* L);
#endif

