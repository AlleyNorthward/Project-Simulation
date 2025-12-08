#include "transform.hpp"

PySqList::PySqList(){
    InitList_Sq(&L);
}

PySqList::~PySqList(){
    DestroyList_Sq(&L);
}

int PySqList::size() const {
    return L.size;
}

int PySqList::capacity() const {
    return L.capacity;
}











