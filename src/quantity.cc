//# quantity.cc: python module for Quantum<Vector<Double> > objects.
//# Copyright (C) 2007
//# Australia Telescope National Facility, AUSTRALIA
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
//# Correspondence concerning pyrap should be addressed as follows:
//#        Internet email: pyrap-devel@googlegroups.com
//#        Postal address: Australia Telescope National Facility
//#                        PO Box 76
//#                        Epping NSW 1710
//#                        AUSTRALIA
//#
//# $Id:$

#include <casacore/casa/Quanta.h>
#include <casacore/casa/Quanta/QLogical.h>
#include <casacore/casa/Quanta/QuantumHolder.h>
#include <casacore/casa/Quanta/MVTime.h>
#include <casacore/casa/Quanta/MVAngle.h>

#include <casacore/casa/Containers/Record.h>
#include <casacore/casa/Exceptions/Error.h>
#include <casacore/casa/sstream.h>
#include <casacore/casa/BasicSL/String.h>

#include<pybind11/pybind11.h>
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <boost/python/overloads.hpp>

namespace py = pybind11;


namespace casacore {
  namespace python {

  Quantity fromString(const String& str) {
    QuantumHolder qh;
    String err;
    if ( !qh.fromString(err, str) ) {
      throw(AipsError(err));
    }
    return qh.asQuantity();
  }


  String printTime(const Quantity& q, const String& fmt, uInt prec) {
    MVTime mvt(q);
    if (fmt.empty()) {
      return mvt.string(prec);
    }
    return mvt.string(MVTime::giveMe(fmt), prec);
  }

  String printAngle(const Quantity& q, const String& fmt, uInt prec) {
    MVAngle mva(q);
    if (fmt.empty()) {
      return mva.string(prec);
    }
    return mva.string(MVAngle::giveMe(fmt), prec);
  }

  String printQuantum(const Quantity& q,  const String& fmt="", uInt prec=0) {
    if (q.get().getFullUnit() == Unit("s")) {
      return printTime(q, fmt, prec);
    } else if (q.get().getFullUnit() == Unit("rad")) {
      return printAngle(q, fmt, prec);
    }
    ostringstream oss;
    q.print(oss);
    return String(oss);
  }
  // Introduce the overloaded PrintQuantum function
  BOOST_PYTHON_FUNCTION_OVERLOADS(printQuantumOVL, printQuantum, 1, 3)

  // these functions take Unit as argument, enable outside access through
  // strings
  Quantity getWithUnit(const Quantity& q, const String& u)  {
    Unit unit(u);
    return q.get(unit);
  }
  Double getValueWithUnit(const Quantity& q, const String& u)  {
    Unit unit(u);
    return q.getValue(unit);
  }

  Quantity fromRecord(const Record& rec) {
    QuantumHolder qh;
    String err;
    if ( !qh.fromRecord(err, rec) ) {
      throw(AipsError(err));
    }
    return qh.asQuantity();
  }

  bool conforms(const Quantity& left, const Quantity& right) {
    return (left.getFullUnit().getValue() == right.getFullUnit().getValue());
  }

  Record toRecord(const Quantity& q) {
    QuantumHolder qh(q);
    String err;
    Record rec;
    if ( !qh.toRecord(err, rec) ) {
      throw(AipsError(err));
    }
    return rec;
  }

  Quantity toTime(const Quantity& q) {
    if (q.check(UnitVal::TIME)) {
      return q;
    } else {
      Quantity q0 = MVTime(q).get();
      return q0;
    }
  }

  Quantity toAngle(const Quantity& q) {
    if (q.check(UnitVal::ANGLE)) {
      return q;
    } else {
      Quantity q0 = MVAngle(q).get();
      return q0;
    }
  }

    Double toUnixTime(const Quantity& q) {
      // MJD = JD - 2400000.5
      // unix = (JD - 2440587.5) * 86400.0
      const Double mjdsecToUnixsec = (2400000.5 - 2440587.5) * 86400.0;
      Quantity qt = toTime(q);
      return qt.get().getValue() + mjdsecToUnixsec;
    }

    Quantity norm(const Quantity& self, Double a) {
      return Quantity(MVAngle(self)(a).degree(), "deg");
    }

}}

namespace casacore { namespace python {
  void quantity(py::module& m)
  {
    py::class_<Quantity> (m, "Quantity")
      .def (py::init< >())
      .def (py::init< const Quantity& > ())
      .def (py::init< Double, const String& >())
      .def ("__repr__", &printQuantum,
                    py::arg("self"),
					py::arg("fmt")="",
					py::arg("precision")=0)
      .def ("get_value", (const Double& ( Quantity::* )( ) const)(&Quantity::getValue),
	    py::return_value_policy < py::copy_const_reference> ()
	    )
      .def ("get_value", &getValueWithUnit)
      .def ("get_unit", &Quantity::getUnit,
	    py::return_value_policy < py::copy_const_reference> ())
      .def ("convert", (void ( Quantity::* )( const Quantity& ) )(&Quantity::convert))
      .def ("convert", (void ( Quantity::* )( ) )(&Quantity::convert))
      .def ("set_value", &Quantity::setValue)
      .def ("get", (Quantity ( Quantity::* )( ) const)(&Quantity::get))
      .def ("canonical", (Quantity ( Quantity::* )( ) const)(&Quantity::get))
      .def ("get", (Quantity ( Quantity::* )( const Quantity& ) const)(&Quantity::get))
      .def ("get", &getWithUnit)
      .def ("conforms", &conforms)
      .def ("totime", &toTime)
      .def ("to_time", &toTime)
      .def ("toangle", &toAngle)
      .def ("to_angle", &toAngle)
      .def ("to_unix_time", &toUnixTime)
      .def ("to_dict", &toRecord)
      .def ("norm", &norm,  (py::arg("self"), py::arg("a")=-0.5))
      .def (-py::self)
      .def (py::self - py::self)
      .def (py::self -= py::self)
      .def (py::self -= Double())
      .def (py::self - Double() )
      .def (Double() - py::self)
      .def (+py::self)
      .def (py::self + py::self)
      .def (py::self += py::self)
      .def (py::self += Double())
      .def (py::self + Double() )
      .def (Double() + py::self)
      .def (py::self * py::self)
      .def (py::self *= py::self)
      .def (py::self *= Double())
      .def (py::self * Double() )
      .def (Double() * py::self)
      .def (py::self / py::self)
      .def (py::self /= py::self)
      .def (py::self /= Double())
      .def (py::self / Double() )
      .def (Double() / py::self)
      .def (py::self == py::self)
      .def (py::self == Double())
      .def (Double() == py::self)
      .def (py::self != py::self)
      .def (py::self != Double())
      .def (Double() != py::self)

      .def (py::self < py::self)
      .def (py::self < Double())
      .def (Double() < py::self)
      .def (py::self <= py::self)
      .def (py::self <= Double())
      .def (Double() <= py::self)

      .def (py::self > py::self)
      .def (py::self > Double())
      .def (Double() > py::self)
      .def (py::self >= py::self)
      .def (py::self >= Double())
      .def (Double() >= py::self)
      .def ("formatted", &printQuantum, printQuantumOVL(py::arg("q"),
                                                         py::arg("fmt")="",
							 py::arg("precision")=0))
      ;
    py::def ("from_string", &fromString);
    py::def ("from_dict", &fromRecord);

  }
}}
