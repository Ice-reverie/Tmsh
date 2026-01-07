/**********************************************************************
 * @file   GraphDataProviderAPI.h
 * @brief  接口
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef _GraphDataProviderAPI_H
#define _GraphDataProviderAPI_H

#include <QtCore/QtGlobal>

#if defined(GraphDataProvider_API)
#define GraphDataProviderAPI Q_DECL_EXPORT
#else
#define GraphDataProviderAPI Q_DECL_IMPORT
#endif

#endif
