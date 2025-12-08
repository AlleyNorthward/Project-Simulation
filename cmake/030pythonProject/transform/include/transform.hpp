#ifndef _TRANSFORM_HPP
#define _TRANSFORM_HPP

extern "C" {
    #include "sqlist.h"
}

class PySqList{
public:
    PySqList();
    ~PySqList();
    int size() const;
    int capacity() const;
private:
    SqList L;
};

#endif











