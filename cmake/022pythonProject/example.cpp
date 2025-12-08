#include <pybind11/pybind11.h>

#include "sqlist.h"
namespace py = pybind11;

PYBIND11_MODULE(example, m) {
    m.def("sqlist", &sqlist, "A function that adds two numbers");
}
