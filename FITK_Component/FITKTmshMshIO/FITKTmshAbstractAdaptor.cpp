#include "FITKTmshAbstractAdaptor.h"

namespace Tmsh
{
    void FITKTmshAbstractAdaptor::setFileReader(Interface::FITKAbstractTextReader* reader)
    {
        //设置读取器
        _reader = reader;
    }
    void FITKTmshAbstractAdaptor::setFileWriter(Interface::FITKAbstractTextWriter* writer)
    {
        //设置写出器
        _writer = writer;
    }
}


