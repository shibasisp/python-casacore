//# fit.cc: python module for fitting proxy object.
//# Copyright (C) 2006
//# Associated Universities, Inc. Washington DC, USA.
//#
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//#
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//#
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//#
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA
//#
//# $Id: pyfit.cc,v 1.2 2007/03/08 22:51:10 mmarquar Exp $

#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <pybind11/pybind11.h>

#include <casacore/python/Converters/PycBasicData.h>
#include <casacore/python/Converters/PycRecord.h>

#include <casacore/scimath/Fitting/FittingProxy.h>

namespace py = pybind11;

namespace casacore { namespace python {
  void fit(py::module& m)
  {
    py::class_<FittingProxy> (m, "fitting")
      .def (py::init<>())
      .def ("getid", &FittingProxy::getid)
      .def ("getstate", &FittingProxy::getstate)
      .def ("init", &FittingProxy::init)
      .def ("done", &FittingProxy::done)
      .def ("reset", &FittingProxy::reset)
      .def ("set", &FittingProxy::set)
      .def ("functional", &FittingProxy::functional,
 	    (py::arg("id"),
	     py::arg("fnct"),
	     py::arg("x"),
 	     py::arg("y"),
 	     py::arg("wt"),
 	     py::arg("mxit"),
 	     py::arg("constraint")))
      .def ("linear", &FittingProxy::linear,
 	    (py::arg("id"),
	     py::arg("fnct"),
	     py::arg("x"),
 	     py::arg("y"),
 	     py::arg("wt"),
 	     py::arg("constraint")))
      .def ("cxfunctional", &FittingProxy::cxfunctional,
 	    (py::arg("id"),
	     py::arg("fnct"),
	     py::arg("x"),
 	     py::arg("y"),
 	     py::arg("wt"),
 	     py::arg("mxit"),
 	     py::arg("constraint")))
      .def ("cxlinear", &FittingProxy::cxlinear,
 	    (py::arg("id"),
	     py::arg("fnct"),
	     py::arg("x"),
 	     py::arg("y"),
 	     py::arg("wt"),
 	     py::arg("constraint")))
        ;
  }
}}
