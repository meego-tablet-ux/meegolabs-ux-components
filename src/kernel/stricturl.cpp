/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QUrl>
#include <qdeclarative.h>

#include "stricturl.h"

StrictUrl::StrictUrl(QObject *parent): QObject(parent)
{
}

StrictUrl::~StrictUrl()
{
}

QUrl StrictUrl::url() const
{
    return m_url;
}

void StrictUrl::setUrlEncoded(const QString &str)
{
    QUrl url;
    url.fromEncoded(str.toAscii());
    if (url != m_url) {
        m_url = url;
        emit urlChanged();
    }
}

void StrictUrl::setUrlUnencoded(const QString &str)
{
    QUrl url;
    url.setUrl(str, QUrl::StrictMode);
    if (url != m_url) {
        m_url = url;
        emit urlChanged();
    }
}

QML_DECLARE_TYPE(StrictUrl);
