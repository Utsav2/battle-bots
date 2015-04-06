#include <boost/python.hpp>

char const* yay()
{
  return "Hello friend !!!!";
}

BOOST_PYTHON_MODULE(libyay)
{
  using namespace boost::python;
  def("yay", yay);
}
