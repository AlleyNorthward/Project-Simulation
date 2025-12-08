#include <pybind11/pybind11.h>
#include "sqlist.h"

namespace py = pybind11;

int add(int a, int b) {
    return a + b;
}

PYBIND11_MODULE(example, m) {
    m.def("add", &add, "A function that adds two numbers");
    m.def("sqlist", &sqlist, "?");
}

