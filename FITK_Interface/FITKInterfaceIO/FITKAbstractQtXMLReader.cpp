#include "FITKAbstractQtXMLReader.h"
#include <QFile>
#include <QDomDocument>

namespace Interface
{
    
    FITKAbstractQtXMLReader::~FITKAbstractQtXMLReader()
    {
        //关闭文件
        this->closeFile();
        //释放内存
        if (_document) delete _document;
        if (_file) delete _file;
    }

    bool FITKAbstractQtXMLReader::createContent()
    {
        //打开文件
        QFile *_file = new QFile(_fileName);
        if (!_file->open(QFile::ReadOnly )) return false;
        
        //读取文件
        _document = new QDomDocument;
        if (!_document->setContent(_file)) return false;
        //关闭文件
        this->closeFile();
        return true;
    }

    void FITKAbstractQtXMLReader::closeFile()
    {
        if (_file) _file->close();
    }

}


