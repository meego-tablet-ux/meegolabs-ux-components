/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "translator.h"

#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QCoreApplication>

Translator::Translator(QObject *parent) :
    QObject(parent),
    mTranslator(new QTranslator(this)),
    mLangCode(QLocale::system().name())
{
}

QString Translator::getCatalog() const
{
    return mCatalog;
}

void Translator::setCatalog(const QString &catName)
{
    mCatalog = catName;
    loadCatalog();
}

QString Translator::getSearchDir() const
{
    return mSearchDir;
}

void Translator::setSearchDir(const QString &dirName)
{
    mSearchDir = dirName;
    loadCatalog();
}

QString Translator::getLangCode() const
{
    return mLangCode;
}

void Translator::setLangCode(const QString &langCode)
{
    if (langCode.isEmpty())
        resetLangCode();
    else
        mLangCode = langCode;

    loadCatalog();
}

//Private functions

void Translator::loadCatalog()
{
    bool foundIt = false;
    QString catName = QString("%1_%2").arg(mCatalog, mLangCode);
    QString catDir = QString("/usr/share/%1").arg(mCatalog);

    if (mCatalog.isEmpty())
        return;
    if (!(foundIt = mTranslator->load(catName, mSearchDir))) {
        if (!(foundIt = mTranslator->load(catName, catDir))) {
            foundIt = mTranslator->load(catName, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        }
    }

    if (foundIt)
        QCoreApplication::installTranslator(mTranslator);
}
