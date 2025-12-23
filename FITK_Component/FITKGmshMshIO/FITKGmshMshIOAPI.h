 /**********************************************************************
  * @file   FITKGmshMshIOAPI.h
  * @brief  外部接口声明
  * @author liuzhonghua (liuzhonghuaszch@163.com)
  * @date   2024-11-28
  *********************************************************************/
#ifndef _FITKIGMSH_MSHIO_API_H_
#define _FITKIGMSH_MSHIO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGmshMshIO_API)
#define FITKGmshMshIOAPI Q_DECL_EXPORT
#else
#define FITKGmshMshIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKGMSHMSHIOVERSION
#define FITKGMSHMSHIOVERSION "1.0.0"
#endif

#endif // _FITKIGMSH_MSHIO_API_H_