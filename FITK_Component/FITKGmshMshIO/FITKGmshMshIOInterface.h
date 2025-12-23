/**********************************************************************
 * @file   FITKGmshMshIOInterface.h
 * @brief  gmsh msh网格读写接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-28
 *********************************************************************/
#ifndef _FITK_GMSH_MEH_IO_INTERFACE_H___
#define _FITK_GMSH_MEH_IO_INTERFACE_H___

#include <QObject>
#include "FITKGmshMshIOAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"

namespace Interface
{
    class FITKAbstractIO;
}

namespace Gmsh
{
    class FITKGmshMshIOAPI FITKGmshMshIOInterface : public QObject,
        public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        /**
         * @brief  构造函数
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        explicit FITKGmshMshIOInterface() = default;
        /**
         * @brief    析构函数
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-28
         */
        virtual ~FITKGmshMshIOInterface() = default;
        /**
         * @brief  获取组件名称
         * @return 组件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        QString getComponentName() override;
        /**
         * @brief 设置文件名称
         * @param fileName 文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-28
         */
        void setFileName(const QString& fileName);
        /**
         * @brief  执行组件
         * @return bool 执行结果
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool exec(const int indexPort) override;
        /**
         * @brief 是否在独立线程执行读写
         * @param[i]  inThread       是否在独立线程执行读写操作
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void runInThread(const bool & inThread = true);
        /**
         * @brief 需要过滤的维度
         * @param[i]  dim   过滤的维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void setReadFilterDim(QList<int> dim);
        /**
         * @brief    设置是否过滤低维单元数据
         * @param[i] isFilter
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void setFilterLowDimElement(bool isFilter);
        /**
         * @brief 是否正在执行
         * @return true
         * @return false
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isRunning();
        /**
         * @brief 执行是否成功
         * @return true
         * @return false
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool isSuccess();

    signals:
        /**
         * @brief 读写线程结束信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        void ioThreadFinishedSig();

    private:
        /**
         * @brief 文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QString _fileName{};
        /**
         * @brief 是否在线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool _runInThread{ true };
        /**
         * @brief 是否执行成功标记
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        bool _success{ false };
        /**
         * @brief  是否过滤包含的低阶单元
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        bool _isFilterElement{ false };
        /**
         * @brief  被过滤的维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        QList<int> _filterDim{};
        /**
         * @brief 读写线程
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-09-19
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> _io{ nullptr };
    };
}


#endif
