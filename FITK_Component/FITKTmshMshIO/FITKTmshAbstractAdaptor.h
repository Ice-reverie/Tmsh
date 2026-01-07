/**********************************************************************
 * @file   FITKTmshAbstractAdaptor.h
 * @brief  抽象的Tmsh文件读写适配器
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-11-29
 *********************************************************************/
#ifndef __FITKTMSHABSTRACTADAPTOR_R_H__
#define __FITKTMSHABSTRACTADAPTOR_R_H__

#include "FITK_Kernel/FITKAdaptor/FITKAbstractIOAdaptor.h"

namespace Interface
{
    class FITKAbstractTextReader;
    class FITKAbstractTextWriter;
}

namespace Tmsh
{
    /**
     * @brief  抽象的Tmsh文件读写适配器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2024-11-29
     */
    class FITKTmshAbstractAdaptor : public Adaptor::FITKAbstractIOAdaptor
    {
        FITKCLASS(Tmsh, FITKTmshAbstractAdaptor);
    public:
        explicit FITKTmshAbstractAdaptor() = default;
        virtual ~FITKTmshAbstractAdaptor() = default;
        /**
          * @brief 设置INP文件读取器
          * @param reader INP读取类
          * @author liuzhonghua (liuzhonghuaszch@163.com)
          * @date   2024-11-29
          */
        void setFileReader(Interface::FITKAbstractTextReader* reader);
        /**
         * @brief 设置INP文件写出器
         * @param writer INP写出类
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        void setFileWriter(Interface::FITKAbstractTextWriter* writer);

    protected:
        /**
         * @brief INP文件读取器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        Interface::FITKAbstractTextReader* _reader{};

        /**
         * @brief INP文件写出器
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2024-11-29
         */
        Interface::FITKAbstractTextWriter* _writer{};
    };
}


#endif
