/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MUSICINDICATOR_H
#define MUSICINDICATOR_H

#include <QDBusServiceWatcher>
#include <QString>
#include <QStringList>

class MusicServiceProxy;

class MusicIndicator : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QString state READ state NOTIFY stateChanged);

public:
    explicit MusicIndicator(QObject *parent = NULL);
    ~MusicIndicator();

    QString state() const;

signals:
    void stateChanged();

private:
    bool innerIsValid() const;
    MusicServiceProxy *m_innerMI;
    QDBusServiceWatcher *m_serviceWatcher;

private slots:
    void serviceRegistered();
    void serviceUnregistered();
};


#endif // MUSICINDICATOR_H
