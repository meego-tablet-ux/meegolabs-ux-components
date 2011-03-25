/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef VALUESPACEPUBLISHER_H
#define VALUESPACEPUBLISHER_H

#include <QtDeclarative/QDeclarativeParserStatus>

#include <QValueSpacePublisher>

QTM_USE_NAMESPACE

class ValueSpacePublisher: public QObject , public QDeclarativeParserStatus
{
    Q_OBJECT

public:
    ValueSpacePublisher(QObject * parent = 0);
    ~ValueSpacePublisher();

    Q_INTERFACES(QDeclarativeParserStatus);

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged);

    virtual void classBegin() {}

    virtual void componentComplete(){}

    QString path() const { return m_path; }

    void setPath(const QString path) ;

signals:
    void pathChanged();

public slots:
    void setValue(const QString & name, const QVariant & data );

private:
    QValueSpacePublisher * m_publisher;

    QString m_path;
};

#endif
