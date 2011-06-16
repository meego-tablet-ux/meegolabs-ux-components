/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_LOCALE_GLOBAL_H
#define MEEGO_LOCALE_GLOBAL_H

#include <QtCore/qglobal.h>


#if defined(LOCALE_PLUGIN)
#  define LOCALESHARED_EXPORT /**/
#elif defined(LOCALE_LIBRARY)
#  define LOCALESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOCALESHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // MEEGO_LOCALE_GLOBAL_H
