 /**********************************************************************
  * @file   FITKTmshMshIOAPI.h
  * @brief  外部接口声明
  * @author liuzhonghua (liuzhonghuaszch@163.com)
  * @date   2024-11-28
  *********************************************************************/
#ifndef _FITKITMSH_MSHIO_API_H_
#define _FITKITMSH_MSHIO_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKTmshMshIO_API)
#define FITKTmshMshIOAPI Q_DECL_EXPORT
#else
#define FITKTmshMshIOAPI Q_DECL_IMPORT
#endif

#ifndef FITKTmshMSHIOVERSION
#define FITKTmshMSHIOVERSION "1.0.0"
#endif

#endif // _FITKITmsh_MSHIO_API_H_