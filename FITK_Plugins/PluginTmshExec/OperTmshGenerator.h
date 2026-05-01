/**********************************************************************
 * @file   OperTmshGenerator.h
 * @brief  Tmsh网格生成操作�??
 * @date   2025-12-22
 *********************************************************************/
#ifndef __OPERTMSHGENERATOR____
#define __OPERTMSHGENERATOR____

#include "FITK_Kernel/FITKCore/FITKActionOperator.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"

namespace Interface
{
    class FITKAbstractMesherDriver;
}

namespace ModelOper
{
    class OperTmshGenerator : public Core::FITKActionOperator
    {
        Q_OBJECT;
    public:
        explicit OperTmshGenerator() = default;
        virtual ~OperTmshGenerator();

        virtual bool execGUI() override;
        virtual bool execProfession() override;

    private:
        void meshGenOper();
        void tetGenOper();

        static bool _isMeshFinishedExecuting;
        //bool _isMeshFinishedExecuting = false; // ����Ƿ�����ִ��meshGenFinished
        
    private slots:
        void meshGenFinished();
        void meshClean();

    private:
        Interface::FITKAbstractMesherDriver* _mesher{};
    };
}

#endif
