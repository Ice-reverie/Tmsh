/**********************************************************************
 * @file   GraphDataAdaptorAPI.h
 * @brief  接口
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef _GraphDataAdaptorAPI_H
#define _GraphDataAdaptorAPI_H

#include <QtCore/QtGlobal>

#if defined(GraphDataAdaptor_API)
#define GraphDataAdaptorAPI Q_DECL_EXPORT
#else
#define GraphDataAdaptorAPI Q_DECL_IMPORT
#endif

#endif
