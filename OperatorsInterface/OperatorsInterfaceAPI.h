/**********************************************************************
 * @file   OperatorsInterfaceAPI.h
 * @brief  接口
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-03-12
 *********************************************************************/
#ifndef _OperatorsInterfaceAPI_H_
#define _OperatorsInterfaceAPI_H_

#include <QtCore/QtGlobal>


#ifdef OperatorsInterface_API
#define OperatorsInterfaceAPI Q_DECL_EXPORT
#else
#define OperatorsInterfaceAPI Q_DECL_IMPORT
#endif

#endif
