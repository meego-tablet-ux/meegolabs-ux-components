#ifndef LOCALE_GLOBAL_H
#define LOCALE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOCALE_LIBRARY)
#  define LOCALESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOCALESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOCALE_GLOBAL_H
