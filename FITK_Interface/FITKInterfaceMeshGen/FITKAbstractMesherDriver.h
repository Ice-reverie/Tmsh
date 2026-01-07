/**
 *
 * @file FITKAbstractMesherDriver.h
 * @brief 网格驱动器抽象类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-07-18
 *
 */
#ifndef __FITK_ABSTRACT_MESHER_DRIVER_H___
#define __FITK_ABSTRACT_MESHER_DRIVER_H___

#include "FITKInterfaceMeshGenAPI.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include <QObject>

namespace Core
{
    class FITKAbstractDataObject;
}

namespace Interface
{
    /**
     * @brief 网格驱动器抽象类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-07-18
     */
    class FITKInterfaceMeshGenAPI FITKAbstractMesherDriver : public QObject,
        public Core::FITKVarientParams 
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct a new FITKAbstractMesherDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        explicit FITKAbstractMesherDriver() = default;
        /**
         * @brief Destroy the FITKAbstractMesherDriver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual ~FITKAbstractMesherDriver() = 0;
        /**
         * @brief 设置临时数据对象
         * @param[i]  data           临时数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void setDataObject(Core::FITKAbstractDataObject* data);
        /**
         * @brief 设置启动参数
         * @param[i]  args           参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void setArgs(const QStringList& args);
        /**
         * @brief 获取启动参数（由 setArgs 设置）
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-12-22
         */
        QStringList getArgs() const;
        /**
         * @brief 启动网格驱动器
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void startMesher(QStringList info = QStringList()) = 0;
        /**
         * @brief 停止网格驱动器
         * @param[i]  info           附加信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        virtual void stopMesher(QStringList info = QStringList()) = 0;

        /**
         * @brief 插入数据对象，用于提取网格数据
         * @param[i]  key
         * @param[i]  value
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-14
         */
        virtual void insertDataObject(QString key, Core::FITKAbstractDataObject* value);

    signals:
        /**
         * @brief 网格划分结束信号
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        void mesherFinished();

    protected:
        /**
         * @brief 临时数据对象
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        Core::FITKAbstractDataObject* _data{};
        /**
         * @brief 启动参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-07-18
         */
        QStringList _args{};
        /**
         * @brief 数据对象，用于提取网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date 2024-10-14
         */
        QHash<QString, Core::FITKAbstractDataObject*> _dataObject{};
    };

}


#endif
