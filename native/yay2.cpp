#include <boost/python.hpp>

char const* yay2()
{
  return "Hello friend!";
}

BOOST_PYTHON_MODULE(libyay2)
{
  using namespace boost::python;
  def("yay2", yay2);
}