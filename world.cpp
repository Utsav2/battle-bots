
#include <boost/python.hpp>

struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};

using namespace boost::python;

BOOST_PYTHON_MODULE(libworld)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
    ;
}

