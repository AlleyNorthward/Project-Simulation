#ifndef _SQLIST_HPP
#define _SQLIST_HPP

extern "C"{
#include "sqlist.h"
}

class PySqList{
private:
    SqList L;
public:
    PySqList(int capacity);
    ~PySqList();
    int size() const;
    int capacity() const;
    void set_size(int _size);
    void set_capacity(int _capacity);
};
#endif
