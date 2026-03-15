#ifndef LUMINA_PRECOMPILED_H
#define LUMINA_PRECOMPILED_H

// Core Qt Headers
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QWindow>
#include <QWidget>
#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QSettings>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QAction>
#include <QMenu>
#include <QListView>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

// X11 / XCB (Only if available)
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtX11Extras/QX11Info>
#endif
#include <xcb/xcb.h>
#include <xcb/ewmh.h>

#endif // LUMINA_PRECOMPILED_H
