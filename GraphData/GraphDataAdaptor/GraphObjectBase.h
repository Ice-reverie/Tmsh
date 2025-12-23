/**********************************************************************
 * @file   GraphObjectBase.h
 * @brief  渲染对象基类
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef GraphObjectBase_H
#define GraphObjectBase_H

#include "GraphDataAdaptorAPI.h"
#include "GraphObjectCommons.h"
#include "FITK_Kernel/FITKCore/FITKVarientParams.h"
#include "FITK_Component/FITKRenderWindowVTK/FITKGraphObjectVTK.h"

#include <QColor>

class vtkDataSet;
class vtkProp;

namespace Comp {
    class FITKGraphRender;
}

namespace Graph
{
    /**
     * @brief  渲染对象基类
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-03-12
     */
    class GraphDataAdaptorAPI GraphObjectBase : public Comp::FITKGraphObjectVTK, public Core::FITKVarientParams
    {
        Q_OBJECT;
        FITKGraphObjectRegist(GraphObjectBase, Comp::FITKGraphObjectVTK);
        FITKCLASS(Graph, GraphObjectBase);
    public:
        /**
         * @brief    构造函数
         * @param[i] dataObj                     渲染数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        GraphObjectBase(Core::FITKAbstractDataObject* dataObj);
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual ~GraphObjectBase();
        /**
         * @brief    更新
         * @param[i] forceUpdate          是否强制刷新
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual void update(bool forceUpdate);
        /**
         * @brief    获取可见性
         * @return   bool                  是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual bool getVisibility();
        /**
         * @brief    更新可见性
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual void updateVisibility();
        /**
         * @brief    更新子模型颜色
         * @param[i] color                    颜色
         * @param[i] otherPar                 其他参数[缺省]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-28
         */
        virtual void updateSubColor(QColor color, QVariant otherPar = QVariant());
        /**
         * @brief    更新模型颜色
         * @param[i] color                    颜色
         * @author   ZhouZhaoYi (zhouzhaoyi)
         * @date     2025-03-31
         */
        virtual void updateModelColor(QColor color = QColor());
        /**
         * @brief    获取渲染层级
         * @return   int                   渲染层级
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        int getRenderLayer();
        /**
         * @brief    获取渲染层级
         * @return   int                   渲染层级
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        int getDataId();
        /**
         * @brief    是否包含演员
         * @param[i] actor          演员
         * @return   bool           是否包含
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        bool contains(vtkProp* actor);
        /**
         * @brief    根据类型获取网格数据
         * @param[i] forceUpdate                 类型
         * @return   vtkDataSet*                 数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        virtual vtkDataSet* getMesh(ShapeType type);
        /**
         * @brief    根据给定的形状类型与VTK单元id获取形状ID
         * @param[i] vtkCellId                      VTK单元ID
         * @param[i] topAbsShapeType                形状类型
         * @return   int                            形状ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual int getShapeIdByVTKCellId(int vtkCellId, ShapeAbsEnum topAbsShapeType);
        /**
         * @brief    更具给定的形状类型与形状id，获取VTK单元id
         * @param[i] shapeId                        形状id
         * @param[i] topAbsShapeType                形状类型
         * @return   const QVector<int>             单元id
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-12-26
         */
        virtual const QVector<int> getVTKCellIdsByShapeId(int shapeId, ShapeAbsEnum topAbsShapeType);
        /**
         * @brief    设置演员可见性
         * @param[i] isVis                  是否可见
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        virtual void setVisible(bool isVis);
        /**
         * @brief    设置拾取模式
         * @param[i] isPick            是否拾取
         * @param[i] type              类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        virtual void setPickModel(bool isPick, ShapeType type);
        /**
         * @brief    设置渲染层
         * @param[i] render                    渲染层
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-16
         */
        void setRender(Comp::FITKGraphRender* render);
        /**
         * @brief    高亮
         * @param[i] otherValue                   额外参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        virtual void highlight(QVariant otherValue = QVariant());
        /**
         * @brief    取消高亮
         * @param[i] otherValue                   额外参数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-13
         */
        virtual void disHighlight(QVariant otherValue = QVariant());
    protected:
        /**
         * @brief    删除VTK对象模版函数
         * @param[i] obj              VTK对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        template <class T>
        void deleteVTKObj(T* obj) {
            if (obj == nullptr) {
                return;
            }

            obj->Delete();
            obj = nullptr;
        }

        /**
         * @brief    删除C++对象模版函数
         * @param[i] obj              C++对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-03-12
         */
        template <class T>
        void deleteObj(T* obj) {
            if (obj == nullptr) {
                return;
            }

            delete pbj;
            obj = nullptr;
        }

        /**
         * @brief    清空VTK数据对象
         * @param[i] obj                     VTK数据对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-04-11
         */
        template <class T>
        void resetVtkObj(T* obj){
            if (obj){
                obj->Reset();
            }
        }
    protected:
        /**
         * @brief  渲染层级
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-03-12
         */
        int _renderLayer = 0;
        /**
         * @brief  渲染层
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-04-16
         */
        Comp::FITKGraphRender* _render = nullptr;
    };
}

#endif