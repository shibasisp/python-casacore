//# pymeas.cc: python module for ImageProxy object.
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
//# $Id: pyimages.cc,v 1.1 2006/09/28 05:55:00 mmarquar Exp $

#include "pyimages.h"

#include <casacore/python/Converters/PycExcp.h>
#include <casacore/python/Converters/PycBasicData.h>
#include <casacore/python/Converters/PycValueHolder.h>
#include <casacore/python/Converters/PycRecord.h>

#include <images/Images/ImageProxy.h>
#include <images/Images/FITSImage.h>
#include <images/Images/MIRIADImage.h>

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(_images)
{
  // Register the required pyrap converters.
  casa::python::register_convert_excp();
  casa::python::register_convert_basicdata();
  casa::python::register_convert_casa_valueholder();
  casa::python::register_convert_casa_record();
  casa::python::register_convert_std_vector<casa::ImageProxy>();

  // Register the FITS and Miriad image types.
  casa::FITSImage::registerOpenFunction();
  casa::MIRIADImage::registerOpenFunction();

  // Make python interface to images.
  casa::python::pyimages();
}
