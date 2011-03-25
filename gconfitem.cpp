#include "gconfitem.h"

GConfItem::GConfItem(QObject* parent):QObject(parent), item(NULL)
{
}

QString GConfItem::key()
{
    if(item) return item->key();

    return "";
}

QVariant GConfItem::value()
{
    if(item) return item->value(m_defaultValue);
    return QVariant();
}

void GConfItem::setValue(QVariant val)
{
    if(item) item->set(val);
}

void GConfItem::setKey(QString key)
{
    if(item) delete item;

    item = new MGConfItem(key, this);
    connect(item,SIGNAL(valueChanged()),this,SLOT(valueChangedSlot()));
}


void GConfItem::valueChangedSlot()
{
    valueChanged(value());
}
