#include "FITKAbstractPlugin.h"

namespace AppFrame
{
    FITKAbstractPlugin::FITKAbstractPlugin(QLibrary* dylibrary)
        :_library(dylibrary)
    {
    }

    FITKAbstractPlugin::~FITKAbstractPlugin()
    {
    }

    void FITKAbstractPlugin::setLibraryPath(const QString& path)
    {
        //文件路径
        _libPath = path;
    }

    QString FITKAbstractPlugin::getLibraryPath() const
    {
        return _libPath;
    }

  
    void FITKAbstractPlugin::install()
    {
        //纯虚函数
    }

    void FITKAbstractPlugin::unInstall()
    {
        //纯虚函数
    }

    QLibrary* FITKAbstractPlugin::getLibrary()
    {
        return _library;
    }

    QString FITKAbstractPlugin::getPluginName()
    {
        return QString();
    }

    bool FITKAbstractPlugin::exec()
    {
        return false;
    }
    bool FITKAbstractPlugin::read(void *filePointer, QString &error, int fileType)
    {
        return false;
    }
    bool FITKAbstractPlugin::write(void *filePointer, QString &error, int fileType)
    {
        return false;
    }

}