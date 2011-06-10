#ifndef TIMESTAMPUPDATER_H
#define TIMESTAMPUPDATER_H

#include <QList>
#include <QTimer>

#include "relativetimestamp.h"

class TimeStampUpdater : public QObject {
    Q_OBJECT
public:
    static TimeStampUpdater &get();
    void append(RelativeTimeStamp*);
    void remove(RelativeTimeStamp*);
    bool isRunning();
public slots:
    void update();
private:
    TimeStampUpdater();

    QTimer mTimer;
    QList<RelativeTimeStamp*> mTimeStamps;
};

#endif // TIMESTAMPUPDATER_H
