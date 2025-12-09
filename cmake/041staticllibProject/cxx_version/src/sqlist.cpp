
#include "sqlist.hpp"

PySqList::PySqList(int capacity){
    InitList_Sq(&L, capacity);
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

void PySqList::set_size(int _size){
    L.size = _size;
}

void PySqList::set_capacity(int _capacity){
    L.capacity = _capacity;
}











