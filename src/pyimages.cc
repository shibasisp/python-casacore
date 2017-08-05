//# pyimages.cc: python module for aips++ images system
//# Copyright (C) 2008
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
//# $Id$

#include <casacore/images/Images/ImageProxy.h>
#include <casacore/python/Converters/PycBasicData.h>
#include <casacore/python/Converters/PycValueHolder.h>
#include <casacore/python/Converters/PycRecord.h>
#include <boost/python.hpp>
#include <boost/python/args.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace casacore { namespace python {

  void pyimages(py::module& m)
  {
    // Note that all constructors must have a different number of arguments.
    py::class_<ImageProxy> (m, "Image")
            // 1 arg: copy constructor
      .def (py::init<ImageProxy>())
	    // 2 arg: concat from image names
      .def (py::init<Vector<String>, Int>())
            // 3 arg: open image or image expression
      .def (py::init<String, String, vector<ImageProxy> >())
	    // 4 arg: concat from images objects
      .def (py::init<std::vector<ImageProxy>, Int, Int, Int>())
            // 8 arg: create image from array
      .def (py::init<ValueHolder, ValueHolder, Record, String, Bool, Bool,
            String, IPosition>())
            // 9 arg: create image from shape
      .def (py::init<IPosition, ValueHolder, Record, String, Bool, Bool,
            String, IPosition, Int>())

      // Member functions.
      // Functions starting with un underscore are wrapped in image.py.
      .def ("_ispersistent", &ImageProxy::isPersistent)
      .def ("_name", &ImageProxy::name,
            (py::arg("strippath")))
      .def ("_shape", &ImageProxy::shape)
      .def ("_ndim", &ImageProxy::ndim)
      .def ("_size", &ImageProxy::size)
      .def ("_datatype", &ImageProxy::dataType)
      .def ("_imagetype", &ImageProxy::imageType)
      .def ("_getdata", &ImageProxy::getData)
      .def ("_getmask", &ImageProxy::getMask)
      .def ("_putdata", &ImageProxy::putData)
      .def ("_putmask", &ImageProxy::putMask)
      .def ("_haslock", &ImageProxy::hasLock,
 	    (py::arg("write")))
      .def ("_lock", &ImageProxy::lock,
 	    (py::arg("write"),
 	     py::arg("nattempts")))
      .def ("_unlock", &ImageProxy::unlock)
      .def ("_attrgroupnames", &ImageProxy::attrGroupNames)
      .def ("_attrcreategroup", &ImageProxy::createAttrGroup,
            (py::arg("groupname")))
      .def ("_attrnames", &ImageProxy::attrNames,
            (py::arg("groupname")))
      .def ("_attrnrows", &ImageProxy::attrNrows,
            (py::arg("groupname")))
      .def ("_attrget", &ImageProxy::getAttr,
            (py::arg("groupname"),
             py::arg("attrname"),
             py::arg("rownr")))
      .def ("_attrgetrow", &ImageProxy::getAttrRow,
            (py::arg("groupname"),
             py::arg("rownr")))
      .def ("_attrgetunit", &ImageProxy::getAttrUnit,
            (py::arg("groupname"),
             py::arg("attrname")))
      .def ("_attrgetmeas", &ImageProxy::getAttrMeas,
            (py::arg("groupname"),
             py::arg("attrname")))
      .def ("_attrput", &ImageProxy::putAttr,
            (py::arg("groupname"),
             py::arg("attrname"),
             py::arg("rownr"),
             py::arg("value"),
             py::arg("unit"),
             py::arg("meas")))
      .def ("_subimage", &ImageProxy::subImage,
            (py::arg("blc"),
             py::arg("trc"),
             py::arg("inc"),
             py::arg("dropdegenerate")))
      .def ("_coordinates", &ImageProxy::coordSys)
      .def ("_toworld", &ImageProxy::toWorld,
            (py::arg("pixel"),
             py::arg("reverseAxes")))
      .def ("_topixel", &ImageProxy::toPixel,
            (py::arg("world"),
             py::arg("reverseAxes")))
      .def ("_imageinfo", &ImageProxy::imageInfo)
      .def ("_miscinfo", &ImageProxy::miscInfo)
      .def ("_unit", &ImageProxy::unit)
      .def ("_history", &ImageProxy::history)
      .def ("_tofits", &ImageProxy::toFits,
            (py::arg("filename"),
             py::arg("overwrite"),
             py::arg("velocity"),
             py::arg("optical"),
             py::arg("bitpix"),
             py::arg("minpix"),
             py::arg("maxpix")))
      .def ("_saveas", &ImageProxy::saveAs,
            (py::arg("filename"),
             py::arg("overwrite"),
             py::arg("hdf5"),
             py::arg("copymask"),
             py::arg("newmaskname"),
             py::arg("newtileshape")))
      .def ("_statistics", &ImageProxy::statistics,
            (py::arg("axes"),
             py::arg("mask"),
             py::arg("minMaxValues"),
             py::arg("exclude"),
             py::arg("robust")))
      .def ("_regrid", &ImageProxy::regrid,
            (py::arg("axes"),
             py::arg("outname"),
             py::arg("overwrite"),
             py::arg("outshape"),
             py::arg("coordsys"),
             py::arg("interpolation"),
             py::arg("decimate"),
             py::arg("replicate"),
             py::arg("refchange"),
             py::arg("forceregrid")))
    ;
  }

}}
