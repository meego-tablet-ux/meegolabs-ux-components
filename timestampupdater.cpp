#include <QDebug>
#include "timestampupdater.h"

//TODO disable timer if app is not in foreground or screenlock is active
//TODO adjust interval based on when timestamps need updating

TimeStampUpdater::TimeStampUpdater()
{
    mTimer.setInterval(60 * 1000); // 1 minute
    mTimer.setSingleShot(false);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(update()));
}

TimeStampUpdater &TimeStampUpdater::get()
{
    static TimeStampUpdater updater;
    return updater;
}

void TimeStampUpdater::append(RelativeTimeStamp *timestamp)
{
    mTimeStamps.append(timestamp);
    if (mTimeStamps.size() > 0 && !isRunning()) {
        mTimer.start();
    }
}

void TimeStampUpdater::remove(RelativeTimeStamp *timestamp)
{
    mTimeStamps.removeAll(timestamp);
    if (mTimeStamps.size() == 0 && isRunning()) {
        mTimer.stop();
    }
}

bool TimeStampUpdater::isRunning()
{
    return mTimer.timerId() != -1;
}

void TimeStampUpdater::update()
{
    RelativeTimeStamp *timestamp;
    foreach (timestamp, mTimeStamps) {
            timestamp->update();
    }
}
