/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -c BrowserServiceInterface -p browserserviceiface.h: BrowserService.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef BROWSERSERVICEIFACE_H_1286877147
#define BROWSERSERVICEIFACE_H_1286877147

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.meego.browser.BrowserService
 */
class BrowserServiceInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.meego.browser.BrowserService"; }

public:
    BrowserServiceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~BrowserServiceInterface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> removeBookmark(const QString &id)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(id);
        return asyncCallWithArgumentList(QLatin1String("removeBookmark"), argumentList);
    }

    inline QDBusPendingReply<> removeUrl(const QString &url)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(url);
        return asyncCallWithArgumentList(QLatin1String("removeUrl"), argumentList);
    }

    inline QDBusPendingReply<> viewItem(const QString &url)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(url);
        return asyncCallWithArgumentList(QLatin1String("viewItem"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void bookmarkRemoved(qint64 id);
    void bookmarkUpdated(qint64 id, const QString &url, const QString &title, const QString &faviconUrl);
    void faviconUpdated(const QString &url);
    void thumbnailUpdated(const QString &url);
    void urlRemoved(const QString &url);
    void urlVisited(qint64 id, const QString &url, const QString &title, const QString &faviconUrl);
};

#endif
