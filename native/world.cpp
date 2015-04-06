#include <boost/python.hpp>
#include <iostream>
struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg; 
	void k(){ std::cout<< " \nchootiye!!"; }
};

using namespace boost::python;

BOOST_PYTHON_MODULE(libworld)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
        .def("k", &World::k)
    ;
}
