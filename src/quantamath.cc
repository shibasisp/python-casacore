//# quantamath.cc: python module for Quantum<Vector<Double> > global math.
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
//# $Id:$

#include <casacore/casa/Quanta.h>
#include <casacore/casa/Quanta/QMath.h>
#include <casacore/casa/Quanta/QLogical.h>
#include <casacore/casa/version.h>
#include <sstream>

#include<pybind11/pybind11.h>
#include <boost/python.hpp>

namespace py = pybind11;

namespace casacore {
  namespace python {

    py::dict constants() {
      py::dict d;
      const uInt N = 20;
      static String types[N] = {
	"pi", "ee", "c", "G", "h", "HI", "R", "NA", "e", "mp",
	"mp_me", "mu0", "epsilon0", "k", "F", "me", "re", "a0",
	"R0", "k2"
      };
      static Quantity res[N] = {
	Quantity(C::pi,""), Quantity(C::e,""),
	QC::c, QC::G, QC::h, QC::HI, QC::R, QC::NA, QC::e, QC::mp,
	QC::mp_me, QC::mu0, QC::epsilon0, QC::k, QC::F, QC::me, QC::re, QC::a0,
	QC::R0, QC::k2
      };
      for (int i=0; i<20;++i) {
	d[py::str(types[i])] = res[i];
      }
      return d;
    }

    py::dict unitMap(map<String, UnitName> mymap) {
      py::dict d;

      for (map<String, UnitName>::iterator i=mymap.begin();
       i != mymap.end(); ++i) {
        py::list unitdesc;
        ostringstream oss;
// Test for casacore > 2.0.3
#if CASACORE_MAJOR_VERSION>2 || (CASACORE_MAJOR_VERSION==2 && \
                 (CASACORE_MINOR_VERSION>0 || (CASACORE_MINOR_VERSION==0 \
                         && CASACORE_PATCH_VERSION>3)))
        // Use the getter introduced in casacore 2.0.4
        unitdesc.append((i->second).getFullName());
#else
        // Do the same thing with some string parsing (yuk)
        oss << i->second;
        string namestring=oss.str();
        unitdesc.append(namestring.substr(11, namestring.rfind(")")-11));
        oss.str("");
#endif
        oss<<(i->second).getVal().getDim();
        Quantity q((i->second).getVal().getFac(),oss.str());
        unitdesc.append(q);
        d[py::str((i->second).getName())] = unitdesc;
      }
      return d;
    }

    py::dict units() {
      map<String, UnitName> mapSI = UnitMap::giveSI();
      map<String, UnitName> mapDef = UnitMap::giveDef();
      map<String, UnitName> mapCust = UnitMap::giveCust();
      mapSI.insert(mapDef.begin(), mapDef.end());
      mapSI.insert(mapCust.begin(), mapCust.end());
      return unitMap(mapSI);
    }

    py::dict prefixes() {
      map<String, UnitName> mapPref = UnitMap::givePref();
      return unitMap(mapPref);
    }


    typedef Quantum<Vector<Double> > QProxy;
    typedef Vector<Double> VD;
    void quantamath(py::module& m)
    {
      // misc
      m.def ("constants", &constants);
      m.def ("units", &units);
      m.def ("prefixes", &prefixes);

      // Quantum<Vector<Double> > functions

      m.def ("nearabs", (Bool ( * )( const QProxy&, const QProxy&,
				   Double ) )(&nearAbs));
      m.def ("nearabs", (Bool ( * )( const VD&, const QProxy&,
				   Double ) )(&nearAbs));
      m.def ("nearabs", (Bool ( * )( const QProxy&, const VD&,
				   Double ) )(&nearAbs));

      m.def ("near", (Bool ( * )( const QProxy&, const QProxy&,
				Double ) )(&near));
      m.def ("near", (Bool ( * )( const VD&, const QProxy&,
				Double ) )(&near));
      m.def ("near", (Bool ( * )( const QProxy&, const VD&,
				Double ) )(&near));

      m.def ("abs", (QProxy ( * )( const QProxy&) )(&abs));
      m.def ("pow", (QProxy ( * )( const QProxy&, Int) )(&pow));
      m.def ("root", (QProxy ( * )( const QProxy&, Int) )(&root));
      m.def ("sqrt", (QProxy ( * )( const QProxy&) )(&sqrt));
      m.def ("ceil", (QProxy ( * )( const QProxy&) )(&ceil));
      m.def ("floor", (QProxy ( * )( const QProxy&) )(&floor));

      m.def ("sin", (QProxy ( * )( const QProxy&) )(&sin));
      m.def ("cos", (QProxy ( * )( const QProxy&) )(&cos));
      m.def ("tan", (QProxy ( * )( const QProxy&) )(&tan));
      m.def ("asin", (QProxy ( * )( const QProxy&) )(&asin));
      m.def ("acos", (QProxy ( * )( const QProxy&) )(&acos));
      m.def ("atan", (QProxy ( * )( const QProxy&) )(&atan));
      m.def ("atan2", (QProxy ( * )( const QProxy&, const QProxy&) )(&atan2));
      m.def ("atan2", (QProxy ( * )( const QProxy&, const VD&) )(&atan2));
      m.def ("atan2", (QProxy ( * )( const VD&, const QProxy&) )(&atan2));

      m.def ("log", (QProxy ( * )( const QProxy&) )(&log));
      m.def ("log10", (QProxy ( * )( const QProxy&) )(&log10));
      m.def ("exp", (QProxy ( * )( const QProxy&) )(&exp));


      // Quantity functions

      m.def ("nearabs", (Bool ( * )( const Quantity&,
				   const Quantity&) )(&nearAbs));
      m.def ("nearabs", (Bool ( * )( const Quantity&, const Quantity&,
				   Double ) )(&nearAbs));
      m.def ("nearabs", (Bool ( * )( const Double&, const Quantity&,
				   Double ) )(&nearAbs));
      m.def ("nearabs", (Bool ( * )( const Quantity&, const Double&,
				   Double ) )(&nearAbs));
      m.def ("near", (Bool ( * )( const Quantity&, const Quantity&) )(&near));

      m.def ("near", (Bool ( * )( const Quantity&, const Quantity&,
				Double ) )(&near));
      m.def ("near", (Bool ( * )( const Double&, const Quantity&,
				Double ) )(&near));
      m.def ("near", (Bool ( * )( const Quantity&, const Double&,
				Double ) )(&near));
      m.def ("abs", (Quantity ( * )( const Quantity&) )(&abs));
      m.def ("pow", (Quantity ( * )( const Quantity&, Int) )(&pow));
      m.def ("root", (Quantity ( * )( const Quantity&, Int) )(&root));
      m.def ("sqrt", (Quantity ( * )( const Quantity&) )(&sqrt));
      m.def ("ceil", (Quantity ( * )( const Quantity&) )(&ceil));
      m.def ("floor", (Quantity ( * )( const Quantity&) )(&floor));

      m.def ("sin", (Quantity ( * )( const Quantity&) )(&sin));
      m.def ("cos", (Quantity ( * )( const Quantity&) )(&cos));
      m.def ("tan", (Quantity ( * )( const Quantity&) )(&tan));
      m.def ("asin", (Quantity ( * )( const Quantity&) )(&asin));
      m.def ("acos", (Quantity ( * )( const Quantity&) )(&acos));
      m.def ("atan", (Quantity ( * )( const Quantity&) )(&atan));
      m.def ("atan2", (Quantity ( * )( const Quantity&,
				     const Quantity&) )(&atan2));
      m.def ("atan2", (Quantity ( * )( const Quantity&,
				     const Double&) )(&atan2));
      m.def ("atan2", (Quantity ( * )( const Double&,
				     const Quantity&) )(&atan2));

      m.def ("log", (Quantity ( * )( const Quantity&) )(&log));
      m.def ("log10", (Quantity ( * )( const Quantity&) )(&log10));
      m.def ("exp", (Quantity ( * )( const Quantity&) )(&exp));
    }
  }
}
