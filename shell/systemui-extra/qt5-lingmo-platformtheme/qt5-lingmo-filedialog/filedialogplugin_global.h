#ifndef FILEDIALOGPLUGIN_GLOBAL_H
#define FILEDIALOGPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT5LINGMOFILEDIALOG_LIBRARY)
#  define FILEDIALOGPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define FILEDIALOGPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // FILEDIALOGPLUGIN_GLOBAL_H
