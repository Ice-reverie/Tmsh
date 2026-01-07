/**********************************************************************
 * @file   FITKCGNSIOAPI.h
 * @brief  网格文件格式cgns读写外部接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2024-09-19
 *********************************************************************/
#ifndef FITKCGNSIOAPI_H
#define FITKCGNSIOAPI_H


#include <QtCore/QtGlobal>


#ifdef FITKCGNSIO_API
#define FITKCGNSIOAPI Q_DECL_EXPORT
#else
#define FITKCGNSIOAPI Q_DECL_IMPORT
#endif

#endif // FITKCGNSIOAPI_H
