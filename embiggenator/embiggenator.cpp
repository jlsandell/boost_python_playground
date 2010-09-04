#include <string>
#include <algorithm>
#include <iostream>

#include <boost/python/operators.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

struct embiggenator {

    std::string msg;

    embiggenator(std::string msg = "") { set_toupper(msg); }

    void set_toupper(const std::string &msg)
    {
        this->msg.resize(msg.size());
        std::transform(msg.begin(), msg.end(), this->msg.begin(), ::toupper);
    }

    void set(const std::string msg) { set_toupper(msg); }

    std::string get() { return this->msg; }
};

std::ostream & operator<<(std::ostream &out, embiggenator em)
{
    out << em.get();
    return out;
}


BOOST_PYTHON_MODULE(embiggenator)
{
    using namespace boost::python;

    class_<embiggenator>("embiggenator", init<optional<std::string> >())
        .def("set", &embiggenator::set)
        .def("get", &embiggenator::get)
        .def(str(self));
}
