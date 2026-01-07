/**********************************************************************
 * @file   FITKGmshAdaptorElements.h
 * @brief  Gmsh msh 单元读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKGMSHADAPTORELEMENTS_R_H__
#define __FITKGMSHADAPTORELEMENTS_R_H__

#include "FITKGmshAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

ForwardDeclarNS(Interface, FITKComponentManager)

namespace Gmsh
{
    /**
     * @brief  Gmsh msh 单元读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKGmshAdaptorElements : public FITKGmshAbstractAdaptor
    {
    public:
        explicit FITKGmshAdaptorElements() = default;
        virtual ~FITKGmshAdaptorElements() = default;

        /**
         * @brief    获取适配器数据类型名
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        QString getAdaptorClass() override;
        /**
         * @brief    设置集合管理器 - 根据读取的单元划分组
         * @param[i] setSurfManager 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-02
         */
        void setComponentManager(Interface::FITKComponentManager* setSurfManager);

        /**
         * @brief    适配器读取
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptR() override;

        /**
         * @brief    适配器写出
         * @return   状态 true成功， false失败
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool adaptW() override;

    private:
        /**
         * @brief    读取所有单元的信息
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readElementsInfo();
        /**
         * @brief    读取单元
         * @param[i] info
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        bool readElement(QStringList info);
        /**
         * @brief    读取集合数据
         * @param[i] elemIdList 
         * @return   bool
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-12-02
         */
        bool readSetData(QList<int> elemIdList);
        /**
         * @brief    获取单元类型
         * @param[i] type 类型
         * @param[o] nodeNum 节点数
         * @param[o] nodeIndexs 节点顺序
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2024-11-29
         */
        int getElementType(int type, int& nodeNum, QList<int>& nodeIndexs);

    private:
        /**
         * @brief  集合管理器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-12-02
         */
        Interface::FITKComponentManager* m_setSurfManager{};
    };

    Register2FITKIOAdaptorFactory(Gmsh, FITKGmshAdaptorElements, FITKGmshAdaptorElements)
}


#endif
