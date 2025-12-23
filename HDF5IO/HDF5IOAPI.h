/**********************************************************************
 * @file   HDF5IOAPI.h
 * @brief  接口声明
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-04-11
 *********************************************************************/
#ifndef _HHDF5_IO_API_H_
#define _HDF5_IO_API_H_

#include <QtCore/QtGlobal>


#if defined(HDF5IO_API)
#define HDF5IOAPI Q_DECL_EXPORT
#else
#define HDF5IOAPI Q_DECL_IMPORT
#endif

#endif
