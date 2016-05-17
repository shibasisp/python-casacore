#include "fitsconverter.h"
#include <iostream>

#include "casacore/images/Images/ImageInterface.h"

std::string get_fits_header(const ImageProxy &imageProxy) {
    String error;
    ImageFITSHeaderInfo fhi;
    LatticeBase* latticePtr = imageProxy.getLattice();
    ImageInterface<Float>* imagePtr = dynamic_cast<ImageInterface<Float>*>(latticePtr);
    AlwaysAssert (imagePtr!=0, AipsError);
    Bool ok = ImageFITSConverter::ImageHeaderToFITS(error, fhi, *imagePtr);
    std::string header = fhi.kw.toString();
    if (!ok) {
            return error;
        } else {
            return header;
    }
}