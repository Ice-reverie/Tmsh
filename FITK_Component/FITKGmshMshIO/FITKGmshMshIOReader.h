/**********************************************************************
 * @file   FITKGmshMshIOReader.h
 * @brief  gmsh msh网格文件读取
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef _FITK_GMSH_MSH_IO_READER_H___
#define _FITK_GMSH_MSH_IO_READER_H___

#include "FITKGmshMshIOAPI.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractTextReader.h"

namespace Interface
{
    class FITKComponentManager;
}

namespace Gmsh
{
    class FITKGmshMshIOReaderTool;
    /**
     * @brief  gmsh msh网格文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshMshIOAPI FITKGmshMshIOReader : public Interface::FITKAbstractTextReader
    {
        Q_OBJECT
    public:
        explicit FITKGmshMshIOReader() = default;
        virtual ~FITKGmshMshIOReader() = default;
        /**
         * @brief    设置集合管理器
         * @param[i] componentManager
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-06
         */
        void setComponentManager(Interface::FITKComponentManager* componentManager);
        /**
         * @brief 重写run函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void run() override;
        /**
         * @brief   设置需要过滤的维度
         * @param[i]  dim   过滤的维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void setReadFilterDim(QList<int> dim);
        /**
         * @brief    获取需要过滤的维度
         * @return   QList<int>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-05-23
         */
        QList<int> getReadFilterDim();
        /**
         * @brief    设置是否过滤低维单元数据
         * @param[i] isFilter 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void setFilterLowDimElement(bool isFilter);
        /**
         * @brief    获取读取工具
         * @return   FITKGmshMshIOReaderTool *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-03
         */
        FITKGmshMshIOReaderTool* getReaderTool();
        /**
         * @brief 打印控制台消息
         * @param[i] level 打印级别 1 normal 2 warning 3error
         * @param[i] message 数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void consoleMessage(int level, const QString& message) override;

    protected:
        /**
         * @brief    开始读取数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-03
         */
        bool read();

    protected:
        /**
         * @brief  被过滤的维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QList<int> _filterDim{};
        /**
         * @brief  集合管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-06
         */
        Interface::FITKComponentManager* _componentMgr{};
        /**
         * @brief  是否过滤包含的低阶单元
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        bool _isFilterElement{ false };
        /**
         * @brief  读取工具
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        FITKGmshMshIOReaderTool* _tool{};
    };
}


#endif
