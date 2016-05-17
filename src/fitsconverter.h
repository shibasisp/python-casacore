#ifndef PYRAP_FITS_H
#define PYRAP_FITS_H

#include "casacore/images/Images/ImageInterface.h"
#include "casacore/images/Images/FITSImage.h"
#include "casacore/images/Images/ImageProxy.h"

using namespace casacore;

std::string get_fits_header(const ImageProxy &image);


#endif //PYRAP_FITS_H
