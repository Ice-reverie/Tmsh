/**********************************************************************
 * @file   HDF5Reader.h
 * @brief  网格HDF5工程文件文件读取
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-11
 *********************************************************************/
#ifndef _FITKMESHHDF5READER_H___
#define _FITKMESHHDF5READER_H___

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"

namespace AppFrame
{
    class FITKAbstractPlugin;
}

namespace IO
{
    class HDF5IOInterface;
    /**
     * @brief  网格HDF5工程文件读取
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-04-11
     */
    class HDF5Reader : public IO::FITKAbstractHDF5Reader
    {
    public:
        /**
         * @brief Construct a new HDF5Reader object
         * @param[i]  interface      组件指针，可传递参数
         * @param[o]  isSuccess      是否执行成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        explicit HDF5Reader(HDF5IOInterface* interface, bool & isSuccess);
        /**
         * @brief Destroy the HDF5Reader object
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        virtual ~HDF5Reader() = default;
        /**
         * @brief 执行读取操作
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        void run() override;
        /**
         * @brief    设置插件数据
         * @param[i] pluginList 插件链表
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        void setPluginData(QList<AppFrame::FITKAbstractPlugin*> pluginList);
        /**
          * @brief    打印控制台消息
          * @param[i] level 打印级别 1 normal 2 warning 3error
          * @param[i] str 打印的信息
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2025-04-11
          */
        void consoleMessage(int level, const QString& str) override;

    private:
        /**
         * @brief    检查HDF5工程文件
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-11
         */
        bool checkVersion();
        /**
         * @brief    读取插件数据
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-13
         */
        bool readPluginData();

    private:
        /**
         * @brief  组件对象指针
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        HDF5IOInterface* _compInterface{};
        /**
         * @brief  插件数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-04-11
         */
        QList<AppFrame::FITKAbstractPlugin*> _pluginList;
        /**
         * @brief 是否执行成功
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date  2025-04-11
         */
        bool & _isSuccess;
    };
}


#endif
