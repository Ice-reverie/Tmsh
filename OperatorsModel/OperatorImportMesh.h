/**
 *
 * @file OperatorsImportModel.h
 * @brief 导入模型
 * @author shenzhuowei (2931275006@qq.com)
 * @date 2024-12-17
 *
 */
#ifndef _OperatorsImportMesh_H
#define _OperatorsImportMesh_H

#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "OperatorsModelAPI.h"
#include "FITK_Kernel/FITKCore/FITKThreadTask.h"

namespace ModelOper
{
    /**
    * @brief 导入模型操作器
    * @author shenzhuowei (2931275006@qq.com)
    * @date 2024-12-17
    */
    class   OperatorsImportMesh :public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        explicit OperatorsImportMesh();
        virtual ~OperatorsImportMesh();

        virtual bool execGUI();

        virtual bool execProfession();
    private slots:
        /**
        * @brief 导入完成槽函数
        * @param[i]  result         是否成功
        * @param[i]  objID          几何id
        * @author shenzhuowei (2931275006@qq.com)
        * @date 2024-12-18
        */
        void slotImportFinish(bool result, int objID);
    };

    ///**
    //* @brief 导入文件读取线程
    //* @author BaGuijun (baguijun@163.com)
    //* @date 2024-08-14
    //*/
    //class ImportReadThread :public Core::FITKThreadTask
    //{
    //    Q_OBJECT;
    //public:
    //    /**
    //     * @brief Construct a new Import Read Thread object
    //     * @author BaGuijun (baguijun@163.com)
    //     * @date 2024-08-14
    //     */
    //    ImportReadThread() = default;
    //    /**
    //     * @brief Destroy the Import Read Thread object
    //     * @author BaGuijun (baguijun@163.com)
    //     * @date 2024-08-14
    //     */
    //    ~ImportReadThread() = default;
    //    /**
    //     * @brief 执行函数
    //     * @author BaGuijun (baguijun@163.com)
    //     * @date 2024-08-14
    //     */
    //    void run();
    //signals:
    //    ;
    //    /**
    //     * @brief 导入完成信号
    //     * @author BaGuijun (baguijun@163.com)
    //     * @date 2024-08-14
    //     */
    //    void sigImportFinish(bool, int);
    //public:
    //    /**
    //     * @brief 文件名称
    //     * @author BaGuijun (baguijun@163.com)
    //     * @date 2024-08-14
    //     */
    //    QString _fileName = "";
    //};

    // 按钮注册相关操作
    Register2FITKOperatorRepo(actionImportMesh, OperatorsImportMesh);
    //Register2FITKOperatorRepo(actionWorkingDir, OperatorsImportMesh);
}

#endif