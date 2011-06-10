#ifndef RELATIVETIMESTAMP_H
#define RELATIVETIMESTAMP_H

#include <QObject>
#include <QDateTime>

class RelativeTimeStamp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime datetime READ getDateTime WRITE setDateTime)
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    RelativeTimeStamp(QObject *parent = 0);
    ~RelativeTimeStamp();

    Q_INVOKABLE QDateTime getDateTime() const;
    Q_INVOKABLE void setDateTime(QDateTime&);
    Q_INVOKABLE const QString text() const;

signals:
    void textChanged();

public slots:
    void update() { emit textChanged(); }

private:
    QDateTime mDatetime;

};

#endif // RELATIVETIMESTAMP_H
