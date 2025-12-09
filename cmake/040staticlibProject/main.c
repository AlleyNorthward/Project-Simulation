#include "sqlist.h"
#include <stdio.h>

int main(){
    SqList L;
    InitList_Sq(&L, 20);
    printf("%d\n", L.size);
    printf("%d", L.capacity);
    return 0;
}











