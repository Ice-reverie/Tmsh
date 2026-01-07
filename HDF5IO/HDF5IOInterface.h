/**********************************************************************
 * @file   HDF5IOInterface.h
 * @brief  网格工程文件读写组件接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-11
 *********************************************************************/
#ifndef _FITKMESHHDF5IOINTERFACE_H__
#define _FITKMESHHDF5IOINTERFACE_H__

#include "HDF5IOAPI.h"
#include "FITK_Kernel/FITKAppFramework/FITKComponentInterface.h"
#include "FITK_Kernel/FITKCore/FITKAbstractObjectContainer.hpp"
#include <QObject>

namespace Interface
{
    class FITKAbstractIO;
}

namespace AppFrame
{
    class FITKAbstractPlugin;
}

namespace IO
{
    /**
     * @brief  网格工程文件读写组件接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-11
     */
    class  HDF5IOAPI HDF5IOInterface : public QObject,
        public AppFrame::FITKComponentInterface
    {
        Q_OBJECT
    public:
        /**
         * @brief  Construct a new HDF5IOInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        explicit HDF5IOInterface() = default;
        /**
         * @brief  Destroy the HDF5IOInterface object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        virtual ~HDF5IOInterface() = default;
        /**
         * @brief    获取部件名称，不能重复  返回 MeshHDF5IO
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        QString getComponentName() override;
        /**
         * @brief    设置文件名称
         * @param[i] fileName 文件名称，要求包含绝对路径
         * @return   void 
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        void setFileName(const QString& fileName);
        /**
         * @brief    设置插件数据
         * @param[i] pluginList 插件链表
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        void setPluginData(QList<AppFrame::FITKAbstractPlugin*> pluginList);
        /**
         * @brief    执行操作
         * @param[i] indexPort 1-读取 2-写出
         * @return   bool 执行是否成功，若多线程读写，返回值无实际意义
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        bool exec(const int indexPort) override;
        /**
         * @brief    是否在独立线程执行读写
         * @param[i] inThread 是否在独立线程执行读写操作
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        void runInThread(const bool & inThread = true);
        /**
         * @brief    是否正在执行
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        bool isRunning();

        /**
         * @brief    执行是否成功
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        bool isSuccess();

    signals:
        /**
         * @brief  读写线程结束信号
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        void ioThreadFinishedSig();

    private:
        /**
         * @brief  文件名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        QString _fileName{};
        /**
         * @brief  是否在线程运行
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        bool _runInThread{ true };
        /**
         * @brief  是否执行成功标记
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        bool _success{ false };
        /**
         * @brief  插件数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        QList<AppFrame::FITKAbstractPlugin*> _pluginList;
        /**
         * @brief  io 对象
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        Core::FITKAbstractObjectContainer<Interface::FITKAbstractIO> _io{ nullptr };
    };
}

#endif
