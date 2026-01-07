#include "FITKOCCModelFromMesh.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include <TopoDS_Shape.hxx>
#include <StlAPI.hxx>
#include <QDebug>

namespace OCC
{
    FITKOCCModelFromMesh::FITKOCCModelFromMesh()
        : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFromMesh::update()
    {
        // 写出stl文件
        QString filePath = FITKAPP->getTempDir("geo");
        filePath += "/mesh.stl";
        if (!writeSTL(filePath))
        {
            qDebug() << "Write STL file failed!";
            return false;
        }

        QByteArray ba = filePath.toLocal8Bit();
        const char* cfileName = ba.data();

        try
        {
            TopoDS_Shape ashape; //读取的结果
            bool  ok = StlAPI::Read(ashape, (const Standard_CString)cfileName);
            if (ok)
            {
                _occShapeAgent->updateShape(ashape);
            }
        }
        catch(...)
        {
            return false;
        }
        
        return true;

    }
}
 