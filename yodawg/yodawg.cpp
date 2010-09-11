/*
 * Example for embedding a Python interpreter.
 * (We heard that you like to code, so we put some code in your code so
 * you can exec while you exec.)
 */

#include <string>
#include <algorithm>
#include <iostream>
#include <cstdlib>

#include <boost/python.hpp>
#include <boost/filesystem.hpp>

/* assume py::whatever is boost::python::whatever.
 * No sense in typing it repeatedly, nor polluting my namespace with it.
 *
 * Same goes for bf/boost::filesystem.
 */

namespace py = boost::python; 
namespace bf = boost::filesystem;

class Random
{
    public:
        int random()
        {
            return 4; // per RFC 1149.5
        }
};

namespace utils 
{
    /**
     *  Wrapper for b::p::import, which doesn't crash the program when it can't find a module.
     *  Sort of like doing
     *  try:
     *      import blah
     *  except ImportError:
     *      blah = None
     */


    py::object loadmodule(const std::string &name)
    {
        try
        {
            return py::import(py::str::str(name));
        }
        catch(const py::error_already_set &err)
        {
            /* we need to fetch the error indicators *before*
             * importing anything, as apparently importing
             * using boost python clears the error flags.
             */

            PyObject *e, *v, *t;
            PyErr_Fetch(&e, &v, &t);

            py::object importError = py::import("exceptions").attr("ImportError");

            /* Squash the exception only if it's an ImportError, otherwise
             * let the exception propagate.
             */
            if (PyErr_GivenExceptionMatches(importError.ptr(), e))
                return py::object(); // None
            else
                throw;
        }
    }

    /* Wrapper around b::p::exec, so I only have to mention the namespace once. 
     * Useful for those cases where the globals and locals are the same.
     */

    py::object exec(const py::str &command, py::object &context)
    {
        return py::exec(command, context, context);
    }
};

int main(int argc, char **argv)
{
    using namespace std;

    try
    {
        Py_Initialize();

        py::object main_module((
                    py::handle<>(py::borrowed(PyImport_AddModule("__main__")))
        ));

        py::object main_namespace = main_module.attr("__dict__");

        main_namespace["Random"] = py::class_<Random>("Random")
                                        .def("random", &Random::random);

        /* Do a little scaffolding to get our script directory into sys.path. */
        py::exec("import sys;from os import path, getcwd; sys.path.append(path.join(getcwd(), 'scripts'))", main_namespace);

        py::object foo = utils::loadmodule("foo");

        if(!foo.is_none())
        {
            main_namespace["foo"] = utils::loadmodule("foo");
            utils::exec("print foo.tester()", main_namespace);
        }

        /* Run our C++ function. */
        utils::exec("random = Random()\nprint random.random()", main_namespace);

        py::object unresolved = utils::loadmodule("unresolved");

        /* Shouldn't run, unless someone gets sneaky and dumps an unresolved.py
         * module into our sys.path. (:
         */
        if (!unresolved.is_none())
        {
            main_namespace["unresolved"] = unresolved;
            utils::exec("print unresolved", main_namespace);
        }
    }
    catch(py::error_already_set)
    {
        PyErr_Print();
    }

    return EXIT_SUCCESS;
}
