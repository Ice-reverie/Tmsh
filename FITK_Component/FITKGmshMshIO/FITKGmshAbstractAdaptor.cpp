#include "FITKGmshAbstractAdaptor.h"

namespace Gmsh
{
    void FITKGmshAbstractAdaptor::setFileReader(Interface::FITKAbstractTextReader* reader)
    {
        //设置读取器
        _reader = reader;
    }
    void FITKGmshAbstractAdaptor::setFileWriter(Interface::FITKAbstractTextWriter* writer)
    {
        //设置写出器
        _writer = writer;
    }
}


