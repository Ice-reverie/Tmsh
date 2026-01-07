/**********************************************************************
 * @file   FITKTmshMshIOReaderTool.h
 * @brief  Tmsh msh文件读取工具
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-03
 *********************************************************************/
#ifndef __FITKTMSHMshIOReaderTool_R_H__
#define __FITKTMSHMshIOReaderTool_R_H__

#include <QVector>
#include <QList>
#include <QHash>

namespace Interface
{
    class FITKUnstructuredMesh;
    class FITKComponentManager;
}

class vtkUnstructuredGrid;

namespace Tmsh
{
    /**
     * @brief  节点数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-03
     */
    class TmshNode
    {
    public:
        int id = 0;
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;

        TmshNode(int id, double x, double y, double z) : id(id), x(x), y(y), z(z) {}
    };
    /**
     * @brief  单元数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-03
     */
    class TmshElement
    {
    public:
        int id = 0;
        int eleType = 0;
        QList<int> nodeIDs;

        TmshElement(int id, int eleType, QList<int> nodeIDs) : id(id), eleType(eleType), nodeIDs(nodeIDs) {}
    };
    /**
     * @brief  集合数据
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-03
     */
    class TmshSet
    {
    public:
        int dim = 0;
        QList<int> elementIDs;

        TmshSet(int dim, QList<int> elementIDs) : dim(dim), elementIDs(elementIDs) {}
    };
    /**
     * @brief  Tmsh msh文件读取工具
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-03
     */
    class FITKTmshMshIOReaderTool
    {
    public:
        explicit FITKTmshMshIOReaderTool(Interface::FITKUnstructuredMesh* mesh, Interface::FITKComponentManager* setSurfManager);
        virtual ~FITKTmshMshIOReaderTool() = default;
        /**
         * @brief    添加点
         * @param[i] id 
         * @param[i] x 
         * @param[i] y 
         * @param[i] z 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void addNode(int id, double x, double y, double z);
        /**
         * @brief    添加单元
         * @param[i] id 
         * @param[i] eleType 
         * @param[i] nodeIDs 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void addElement(int id, int eleType, QList<int> nodeIDs);
        /**
         * @brief    添加集合
         * @param[i] dim 
         * @param[i] elementIDs 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void addElementSet(int dim, QList<int> elementIDs);
        /**
         * @brief    更新
         * @param[i] _isFilterElement 是否过滤掉包含的低维数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void update(bool isFilterElement);

    protected:
        /**
         * @brief    创建集合
         * @param[i] elemIdList 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void createSetData(QList<int> elemIdList);
        /**
         * @brief    开始过滤掉不符合的单元
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void startFilterElement();
        /**
         * @brief    初始化最高维度的单元数据
         * @param[i] mesh 
         * @param[i] nodeMapping 
         * @param[i] elementMapping 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void vtkInitMaxDimElement(vtkUnstructuredGrid* mesh, QHash<int, int>* nodeMapping, QHash<int, int>* elementMapping);
        /**
         * @brief    过滤掉包含的低维数据
         * @param[i] dim 
         * @param[i] mesh 
         * @param[i] nodeMapping 
         * @param[i] elementMapping 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-04
         */
        void vtkFilterLowDimElement(int dim, vtkUnstructuredGrid* mesh, QHash<int, int>* nodeMapping, QHash<int, int>* elementMapping);

    protected:
        /**
         * @brief  节点数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        QVector<TmshNode> _nodes{};
        /**
         * @brief  单元数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        QVector<TmshElement> _elements{};
        /**
         * @brief  单元映射
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        QHash<int, int> _elemMapping{};
        /**
         * @brief  单元集合数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        QVector<TmshSet> _elementSets{};
        /**
         * @brief  最高维度
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-03
         */
        int _maxDim{ 0 };
        /**
         * @brief  记录包含的低维单元的Index
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-04
         */
        QList<int> _containsLowDimElemenet{};
        /**
         * @brief  网格数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-04
         */
        Interface::FITKUnstructuredMesh* _mesh{};
        /**
         * @brief  集合数据
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-04
         */
        Interface::FITKComponentManager* _setSurfManager{};
    };
}


#endif
