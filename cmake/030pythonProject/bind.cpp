#include<pybind11/pybind11.h>
#include "transform.hpp"

namespace py = pybind11;

PYBIND11_MODULE(data_structure, m){
    py::class_<PySqList>(m, "SqList")
        .def(py::init<>())
        .def("size", &PySqList::size)
        .def("capacity", &PySqList::capacity);
}











