/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef STRICTURL_H
#define STRICTURL_H

#include <QObject>
#include <QUrl>

// This class works around a bug in Qt Quick reported here:
// http://bugreports.qt.nokia.com/browse/QTCOMPONENTS-675

class StrictUrl: public QObject
{
    Q_OBJECT

public:
    explicit StrictUrl(QObject *parent = 0);
    ~StrictUrl();

    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged);

    Q_INVOKABLE QUrl url() const;

    Q_INVOKABLE void setUrlEncoded(const QString &str);
    Q_INVOKABLE void setUrlUnencoded(const QString &str);

signals:
    void urlChanged();

protected:
    QUrl m_url;
};

#endif // STRICTURL_H
