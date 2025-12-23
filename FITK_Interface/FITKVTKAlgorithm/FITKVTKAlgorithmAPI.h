/*****************************************************************//**
 * @file   FITKVTKAlgorithmAPI.h
 * @brief  For export macro.
 *  
 * @author ChengHaotian (yeguangbaozi@foxmail.com)
 * @date   2024-03-21
 *********************************************************************/

#ifndef _FITKVTKALGORITHM_API_H_
#define _FITKVTKALGORITHM_API_H_

#include <QtCore/QtGlobal>

#include <QDebug>

#if defined(FITKVTKALGORITHM_API)
#define FITKVTKALGORITHMAPI Q_DECL_EXPORT
#else
#define FITKVTKALGORITHMAPI Q_DECL_IMPORT
#endif

#ifndef FITKVTKAlgorithmVersion
#define FITKVTKAlgorithmVersion "0.8.0"
#endif 

#endif
