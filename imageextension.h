/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef IMAGEEXTENTION_H
#define IMAGEEXTENTION_H
#include <QDeclarativeParserStatus>
#include <QObject>
#include <QImage>
#include <libexif/exif-data.h>
class ImageExtension : public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT
public:
    explicit ImageExtension(QObject *parent = 0);

    ~ImageExtension();

    Q_INTERFACES(QDeclarativeParserStatus);

    Q_PROPERTY(QString source READ source WRITE setSource);
    Q_PROPERTY(int orientation READ orientation NOTIFY orientationChanged);
    Q_PROPERTY(bool hasExif  READ hasExif NOTIFY hasExifChanged);
    Q_PROPERTY(int usrOrientation READ usrOrientation WRITE setUsrOrientation);

public:
    QString source() const {
        return m_source;
    }
    void setSource(const QString source);

    // the orientation here actually contains both orientation and mirror
    //    1	top	left side
    //    2	top	right side
    //    3	bottom	right side
    //    4	bottom	left side
    //    5	left side	top
    //    6	right side	top
    //    7	right side	bottom
    //    8	left side	bottom
    int orientation() const;

    int usrOrientation() const
    {
        return m_usrOrientation;
    }
    void setUsrOrientation(const int usrOrientation)
    {
        if (usrOrientation > 0 && usrOrientation < 9 ){
            m_usrOrientation = usrOrientation;
        }
    }

    bool hasExif() const;

    virtual void classBegin() {}

    virtual void componentComplete(){}

public slots:
    void saveInfo();
signals:
    void orientationChanged();
    void hasExifChanged();
protected:
    QString fixSourceName(const QString source) const;
    void saveIfChanged() const;
private:
    QString m_source;
    ExifData * m_ed;
    int m_usrOrientation;
    QImage *m_image;
};

#endif // IMAGEEXTENTION_H
