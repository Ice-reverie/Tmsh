 
#ifndef _FITKGeoCompOCC_API_API_H_
#define _FITKGeoCompOCC_API_API_H_

#include <QtCore/QtGlobal>


#if defined(FITKGeoCompOCC_API)
#define FITKGeoCompOCCAPI Q_DECL_EXPORT
#else
#define FITKGeoCompOCCAPI Q_DECL_IMPORT
#endif

#ifndef FITKGEOCOMPOCCAPIVERSION
#define FITKGEOCOMPOCCAPIVERSION "1.0.0"
#endif

#endif // _FITKGeoCompOCC_API_API_H_