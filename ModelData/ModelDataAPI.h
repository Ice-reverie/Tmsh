/**
 *
 * @file FITKCoreAPI.h
 * @brief 导入导出库声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-02-22
 *
 */
#ifndef _MODEL_DATA_API_H_
#define _MODEL_DATA_API_H_

#include <QtCore/QtGlobal>


#if defined(ModelData_API)
#define ModelDataAPI Q_DECL_EXPORT
#else
#define ModelDataAPI Q_DECL_IMPORT
#endif

#endif
