//# pytable.cc: python module for TableProxy object.
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
//# $Id: pytable.cc,v 1.5 2006/11/08 00:12:55 gvandiep Exp $

#include <casacore/tables/Tables/TableProxy.h>

#include <casacore/python/Converters/PycBasicData.h>
#include <casacore/python/Converters/PycValueHolder.h>
#include <casacore/python/Converters/PycRecord.h>

#include<pybind11/pybind11.h>
#include <boost/python.hpp>
#include <boost/python/args.hpp>

namespace py = pybind11;

namespace casacore { namespace python {

  void pytable(py::module& m)
  {
    // Note that all constructors must have a different number of arguments.
    py::class_<TableProxy> (m, "Table",
            py::init<>())
	    //  1 arg: copy constructor
      .def (py::init<TableProxy>())
	    //  2 arg: table query command
      .def (py::init<String, std::vector<TableProxy> >())
	    //  3 arg: open single table
      .def (py::init<String, Record, int>())
	    //  4 arg: open multiple tables as concatenation
      .def (py::init<Vector<String>, Vector<String>, Record, int>())
	    //  5 arg: concatenate open tables
      .def (py::init<std::vector<TableProxy>, Vector<String>, int, int, int>())
	    //  7 arg: create new table
      .def (py::init<String, Record, String, String, int, Record, Record>())
	    // 11 arg: read ascii
      .def (py::init<String, String, String, Bool, IPosition, String, String ,int, int, Vector<String>, Vector<String> >())

      // Member functions
      // Functions starting with an underscore are wrapped in table.py.
      .def ("_flush", &TableProxy::flush,
             py::arg("recursive"))
      .def ("_resync", &TableProxy::resync)
      .def ("_close", &TableProxy::close)
      .def ("_toascii", &TableProxy::toAscii,
 	     py::arg("asciifile"),
 	     py::arg("headerfile"),
 	     py::arg("columnnames"),
 	     py::arg("sep"),
 	     py::arg("precision"),
 	     py::arg("usebrackets"))
      .def ("_rename", &TableProxy::rename,
 	     py::arg("newtablename"))
      .def ("_copy", &TableProxy::copy,
 	     py::arg("newtablename"),
 	     py::arg("memorytable"),
	     py::arg("deep"),
 	     py::arg("valuecopy"),
 	     py::arg("endian"),
 	     py::arg("dminfo"),
 	     py::arg("copynorows"))
      .def ("_copyrows", &TableProxy::copyRows,
 	     py::arg("outtable"),
 	     py::arg("startrowin"),
 	     py::arg("startrowout"),
 	     py::arg("nrow"))
      .def ("_selectrows", &TableProxy::selectRows,
 	     py::arg("rownrs"),
 	     py::arg("name"))
      .def ("_iswritable", &TableProxy::isWritable)
      .def ("_endianformat", &TableProxy::endianFormat)
      .def ("_lock", &TableProxy::lock,
 	     py::arg("write"),
 	     py::arg("nattempts"))
      .def ("_unlock", &TableProxy::unlock)
      .def ("_haslock", &TableProxy::hasLock,
 	     py::arg("write"))
      .def ("_lockoptions", &TableProxy::lockOptions)
      .def ("_datachanged", &TableProxy::hasDataChanged)
      .def ("_ismultiused", &TableProxy::isMultiUsed,
 	     py::arg("checksubtables"))
      .def ("_name", &TableProxy::tableName)
      .def ("_partnames", &TableProxy::getPartNames,
 	     py::arg("recursive"))
      .def ("_info", &TableProxy::tableInfo)
      .def ("_putinfo", &TableProxy::putTableInfo,
 	     py::arg("value"))
      .def ("_addreadmeline", &TableProxy::addReadmeLine,
 	     py::arg("value"))
      .def ("_setmaxcachesize", &TableProxy::setMaximumCacheSize,
	     py::arg("columnname"),
	     py::arg("nbytes"))
      .def ("_rownumbers", &TableProxy::rowNumbers,
	     py::arg("table"))
      .def ("_colnames", &TableProxy::columnNames)
      .def ("_isscalarcol", &TableProxy::isScalarColumn,
	     py::arg("columnname"))
      .def ("_coldatatype", &TableProxy::columnDataType,
	     py::arg("columnname"))
      .def ("_colarraytype", &TableProxy::columnArrayType,
	     py::arg("columnname"))
      .def ("_ncols", &TableProxy::ncolumns)
      .def ("_nrows", &TableProxy::nrows)
      .def ("_addcols", &TableProxy::addColumns,
	     py::arg("desc"),
         py::arg("dminfo"),
         py::arg("addtoparent"))
      .def ("_renamecol", &TableProxy::renameColumn,
	     py::arg("oldname"),
	     py::arg("newname"))
      .def ("_removecols", &TableProxy::removeColumns,
	     py::arg("columnnames"))
      .def ("_addrows", &TableProxy::addRow,
	     py::arg("nrows"))
      .def ("_removerows", &TableProxy::removeRow,
	     py::arg("rownrs"))
      .def ("_iscelldefined", &TableProxy::cellContentsDefined,
	     py::arg("columnname"),
	     py::arg("rownr"))
      .def ("_getcell", &TableProxy::getCell,
	     py::arg("columnname"),
	     py::arg("rownr"))
      .def ("_getcellvh", &TableProxy::getCellVH,
	     py::arg("columnname"),
	     py::arg("rownr"),
         py::arg("value"))
      .def ("_getcellslice", &TableProxy::getCellSliceIP,
	     py::arg("columnname"),
	     py::arg("rownr"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"))
      .def ("_getcellslicevh", &TableProxy::getCellSliceVHIP,
	     py::arg("columnname"),
	     py::arg("rownr"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"),
         py::arg("value"))
      .def ("_getcol", &TableProxy::getColumn,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"))
      .def ("_getcolvh", &TableProxy::getColumnVH,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"),
         py::arg("value"))
      .def ("_getvarcol", &TableProxy::getVarColumn,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"))
      .def ("_getcolslice", &TableProxy::getColumnSliceIP,
	     py::arg("columnname"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"))
      .def ("_getcolslicevh", &TableProxy::getColumnSliceVHIP,
	     py::arg("columnname"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"),
         py::arg("value"))
      .def ("_putcell", &TableProxy::putCell,
	     py::arg("columnname"),
	     py::arg("rownr"),
	     py::arg("value"))
      .def ("_putcellslice", &TableProxy::putCellSliceIP,
	     py::arg("columnname"),
	     py::arg("rownr"),
	     py::arg("value"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"))
      .def ("_putcol", &TableProxy::putColumn,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"),
	     py::arg("value"))
      .def ("_putvarcol", &TableProxy::putVarColumn,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"),
	     py::arg("value"))
      .def ("_putcolslice", &TableProxy::putColumnSliceIP,
	     py::arg("columnname"),
	     py::arg("value"),
	     py::arg("blc"),
	     py::arg("trc"),
	     py::arg("inc"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"))
      .def ("_getcolshapestring", &TableProxy::getColumnShapeString,
	     py::arg("columnname"),
	     py::arg("startrow"),
	     py::arg("nrow"),
	     py::arg("rowincr"),
	     py::arg("reverseaxes"))
      .def ("_getkeyword", &TableProxy::getKeyword,
	     py::arg("columnname"),
	     py::arg("keyword"),
	     py::arg("keywordindex"))
      .def ("_getkeywords", &TableProxy::getKeywordSet,
	     py::arg("columnname"))
      .def ("_putkeyword", &TableProxy::putKeyword,
	     py::arg("columnname"),
	     py::arg("keyword"),
	     py::arg("keywordindex"),
	     py::arg("makesubrecord"),
	     py::arg("value"))
      .def ("_putkeywords", &TableProxy::putKeywordSet,
	     py::arg("columnname"),
	     py::arg("value"))
      .def ("_removekeyword", &TableProxy::removeKeyword,
	     py::arg("columnname"),
	     py::arg("keyword"),
	     py::arg("keywordindex"))
      .def ("_getfieldnames", &TableProxy::getFieldNames,
	     py::arg("columnname"),
	     py::arg("keyword"),
	     py::arg("keywordindex"))
      .def ("_getdminfo", &TableProxy::getDataManagerInfo)
      .def ("_getdmprop", &TableProxy::getProperties,
	     py::arg("name"),
	     py::arg("bycolumn"))
      .def ("_setdmprop", &TableProxy::setProperties,
	     py::arg("name"),
         py::arg("properties"),
	     py::arg("bycolumn"))
      .def ("_getdesc", &TableProxy::getTableDescription,
	     py::arg("actual"),
	     py::arg("_cOrder")=true)
      .def ("_getcoldesc", &TableProxy::getColumnDescription,
	     py::arg("columnname"),
 	     py::arg("actual"),
	     py::arg("_cOrder")=true)
      .def ("_showstructure", &TableProxy::showStructure,
	     py::arg("dataman"),
 	     py::arg("column"),
 	     py::arg("subtable"),
	     py::arg("sort"))
      .def ("_getasciiformat", &TableProxy::getAsciiFormat)
      .def ("_getcalcresult", &TableProxy::getCalcResult)
      ;
  }

}}
