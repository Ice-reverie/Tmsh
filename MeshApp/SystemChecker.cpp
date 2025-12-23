#include "SystemChecker.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraph3DWindowInterface.h"

QStringList SystemChecker::check()
{
    QStringList s;

    this->appendError(s, this->checkThreadCount(4));
    this->appendError(s, this->checkMemory(1024 * 4));
    this->appendError(s, this->checkDiskSpace(1024 * 1));
    this->appendError(s, Comp::FITKGraph3DWindowInterface::CheckSystem());

    return s;
}


