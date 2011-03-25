/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef FUZZYDATETIME_H
#define FUZZYDATETIME_H

#include <QObject>
#include <QDateTime>

class FuzzyDateTime : public QObject
{
    Q_OBJECT
public:
    explicit FuzzyDateTime(QObject *parent = 0);

    Q_INVOKABLE const QString getFuzzy(const QDateTime &dt) const;

signals:

public slots:

};

#endif // FUZZYDATETIME_H
