/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -N -c BrowserServiceInterface -i browserserviceiface.h -p :browserserviceiface.cpp BrowserService.xml
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "browserserviceiface.h"
/*
 * Implementation of interface class BrowserServiceInterface
 */

BrowserServiceInterface::BrowserServiceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

BrowserServiceInterface::~BrowserServiceInterface()
{
}

