%module libsalt
#pragma SWIG nowarn=511

#ifdef SWIGPYTHON
%naturalvar;
%rename(node) Node;
%rename(link) Link;
%rename(lane) Lane;
%rename(cell) Cell;
%rename(connection) Connection;
%rename(simulation) Simulation;
%rename(trafficsignal) TrafficSignal;
%rename(vehicle) Vehicle;

%pythoncode %{

def isLibsalt():
    return True

def hasGUI():
    return True

def close():
    simulation.close()

def start(args, outdirprefix=""):
    simulation.load(args, outdirprefix)

def load(args, outdirprefix=""):
    simulation.load(args, outdirprefix)

def simulationStep(step=0):
    simulation.step(step)

def getCurrentStep():
    return simulation.getCurrentStep()

def setCurrentStep(step=0):
    simulation.setCurrentStep(step)

def getBeginStep():
    return simulation.getBeginStep()

def getEndStep():
    return simulation.getEndStep()

%}

#endif

%begin %{
#ifdef _MSC_VER
// ignore constant conditional expression and unreachable code warnings
#pragma warning(disable:4127 4702)
#endif
%}

// replacing vector instances of standard types, see https://stackoverflow.com/questions/8469138
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_pair.i"
%template(StringVector) std::vector<std::string>;
%template(IntVector) std::vector<int>;
%template() std::map<std::string, std::string>;

// exception handling
%include "exception.i"

%exception {
    try {
        $action
    } catch (libsalt::LibsaltException &e) {
        const std::string s = std::string("Error: ") + e.what();
#ifdef SWIGPYTHON
        PyErr_SetObject(SWIG_Python_ExceptionType(SWIGTYPE_p_libsalt__LibsaltException), PyUnicode_FromString(s.c_str()));
        SWIG_fail;
#else
        SWIG_exception(SWIG_ValueError, s.c_str());
#endif
    } catch (std::runtime_error &e) {
        const std::string s = std::string("SALT error: ") + e.what();
        SWIG_exception(SWIG_RuntimeError, s.c_str());
    } catch (...) {
        SWIG_exception(SWIG_UnknownError, "unknown exception");
    }
}


// Add necessary symbols to generated header
%{
#include <libsalt/Simulation.h>
#include <libsalt/Node.h>
#include <libsalt/Link.h>
#include <libsalt/Lane.h>
#include <libsalt/Cell.h>
#include <libsalt/Connection.h>
#include <libsalt/Vehicle.h>
#include <libsalt/TrafficSignal.h>
%}

// Process symbols in header
%include "LibsaltDefs.h"
%include "Simulation.h"
%include "Node.h"
%include "Link.h"
%include "Lane.h"
%include "Cell.h"
%include "Connection.h"
%include "Vehicle.h"
%include "TrafficSignal.h"
