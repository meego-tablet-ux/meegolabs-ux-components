#ifndef GCONFITEM_H
#define GCONFITEM_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <MGConfItem>

class GConfItem: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString key READ key WRITE setKey)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant defaultValue READ defaultValue WRITE setDefaultValue)
public:
    GConfItem(QObject *parent = 0);

    QString key();
    QVariant value();
    QVariant defaultValue() { return m_defaultValue; }
    void setValue(QVariant val);
    void setKey(QString key);
    void setDefaultValue(QVariant val) { m_defaultValue = val; }

public slots:
    void valueChangedSlot();


signals:
    void valueChanged(QVariant value);

private:
    QVariant m_defaultValue;
    MGConfItem* item;
};

#endif // GCONFITEM_H
