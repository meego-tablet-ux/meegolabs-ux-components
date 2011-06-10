/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>

class QTranslator;

class Translator : public QObject
{
    Q_OBJECT
public:
    explicit Translator(QObject *parent = 0);

    Q_PROPERTY(QString catalog READ getCatalog WRITE setCatalog);
    Q_PROPERTY(QString searchDir READ getSearchDir WRITE setSearchDir);
    Q_PROPERTY(QString langCode READ getLangCode WRITE setLangCode);

    Q_INVOKABLE QString getCatalog() const;
    Q_INVOKABLE void setCatalog(const QString &catName);

    Q_INVOKABLE QString getSearchDir() const;
    Q_INVOKABLE void setSearchDir(const QString &dirName);

    Q_INVOKABLE QString getLangCode() const;
    Q_INVOKABLE void setLangCode(const QString &langCode);
    Q_INVOKABLE void resetLangCode();

private:
    void loadCatalog();

    QTranslator *mTranslator;
    QString mCatalog;
    QString mSearchDir;
    QString mLangCode;
};

#endif // TRANSLATOR_H
