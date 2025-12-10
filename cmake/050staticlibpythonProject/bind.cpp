#include<pybind11/pybind11.h>
#include "SqList.hpp"

namespace py = pybind11;

PYBIND11_MODULE(data_structure, m){
    py::class_<PySqList>(m, "SqList")
        .def(py::init<int>())
        .def_property(
            "capacity", 
            &PySqList::capacity,
            &PySqList::set_capacity)
        .def_property(
            "size", 
            &PySqList::size,
            &PySqList::set_size)
        .def("get_capacity", &PySqList::capacity)
        .def("get_size", &PySqList::size)
        ;
}
