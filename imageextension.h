/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef IMAGEEXTENSION_H
#define IMAGEEXTENSION_H
#include <QDeclarativeParserStatus>
#include <QObject>
#include <QImage>
#include <libexif/exif-data.h>

class ImageExtension: public QObject, public QDeclarativeParserStatus
{
    Q_OBJECT

public:
    explicit ImageExtension(QObject *parent = 0);
    ~ImageExtension();

    Q_INTERFACES(QDeclarativeParserStatus);

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged);
    Q_PROPERTY(bool hasExif READ hasExif);
    Q_PROPERTY(int orientation READ orientation NOTIFY orientationChanged);
    Q_PROPERTY(int userOrientation READ userOrientation WRITE setUserOrientation NOTIFY userOrientationChanged);

    enum Orientation {
        TopUp,
        LeftUp,
        BottomUp,
        RightUp
    };
    Q_ENUMS(Orientation)

public:
    QString source() const;
    void setSource(const QString source);

    bool hasExif() const;

    // the orientation specified by the underlying file or overridden value from settings
    int orientation() const;

    // the orientation the user has set through rotation
    int userOrientation() const;
    void setUserOrientation(int userOrientation);

    // TODO: apparently some optimization was intended here
    virtual void classBegin() {}
    virtual void componentComplete() {}

public slots:
    void saveInfo();

signals:
    void sourceChanged();
    void orientationChanged();
    void userOrientationChanged();
    void mirroredChanged();

protected:
    void setOrientation(int orientation);
    QString fixSourceName(const QString source) const;
    void saveIfChanged();
    int exifOrientation() const;

private:
    QString m_source;
    ExifData *m_ed;

    int m_fileOrientation;
    int m_orientation;
    int m_userOrientation;
    bool m_fileMirrored;
};

#endif // IMAGEEXTENSION_H
