/**********************************************************************
 * @file   GraphDataPickAPI.h
 * @brief  接口
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-12-18
 *********************************************************************/
#ifndef _GraphDataPickAPI_H
#define _GraphDataPickAPI_H

#include <QtCore/QtGlobal>

#if defined(GraphDataPick_API)
#define GraphDataPickAPI Q_DECL_EXPORT
#else
#define GraphDataPickAPI Q_DECL_IMPORT
#endif

#endif
