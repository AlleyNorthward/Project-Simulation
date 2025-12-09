#include "sqlist.h"
#include <malloc.h>
#include<stdlib.h>

Status InitList_Sq(SqList* L, int LIST_INIT_SIZE){
    L->base = (ElemType* )malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if(!L->base) exit(OVERFLOW_);

    L->capacity = LIST_INIT_SIZE;
    L->size = 0;
    return OK_;
}

Status DestroyList_Sq(SqList *L){
    if(!L->base) return ERROR_;

    free(L->base);
    L->base = NULL;
    L->size = 0;
    return OK_;
}
