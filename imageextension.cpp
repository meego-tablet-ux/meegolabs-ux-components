/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QSettings>

#include "imageextension.h"

const char Organization[] = "MeeGo";
const char Application[] = "meegolabs-ux-components";
const char Orientations[] = "orientations";

ImageExtension::ImageExtension(QObject *parent) :
        QObject(parent) ,
        m_source(QString()),
        m_ed(NULL),
        m_fileOrientation(TopUp),
        m_orientation(TopUp),
        m_userOrientation(TopUp),
        m_fileMirrored(false)
{
}

ImageExtension::~ImageExtension()
{
    delete m_ed;
}

QString ImageExtension::source() const {
    return m_source;
}

void ImageExtension::setSource(const QString source)
{
    QString fixed_source = fixSourceName(source);
    if (m_source == fixed_source)
        return;
    m_source = fixed_source;

    delete m_ed;
    m_ed = exif_data_new_from_file(m_source.toUtf8().constData());

    int orientation = TopUp;
    m_fileMirrored = false;
    if (m_ed) {
        ExifEntry *entry;
        ExifByteOrder byte_order;
        byte_order = exif_data_get_byte_order(m_ed);

        entry = exif_content_get_entry(m_ed->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION);

        if (entry) {
            // the orientation here actually contains both orientation and mirror
            //    1	top	left side
            //    2	top	right side
            //    3	bottom	right side
            //    4	bottom	left side
            //    5	left side	top
            //    6	right side	top
            //    7	right side	bottom
            //    8	left side	bottom
            short result = exif_get_short(entry->data, byte_order);
            if (result <= 0 || result >= 8)
                result = 1;

            int mapOrientation[] = { 0, TopUp, TopUp, BottomUp, BottomUp,
                                     LeftUp, RightUp, RightUp, LeftUp };
            m_fileOrientation = mapOrientation[result];

            int mapMirrored[] = { 0, 0, 1, 0, 1, 1, 0, 1, 0 };
            m_fileMirrored = mapMirrored[result];
        }
    }

    // check for orientation override
    QSettings settings(Organization, Application);
    settings.beginGroup(Orientations);
    orientation = settings.value(m_source, m_fileOrientation).toInt();

    setOrientation(orientation);
    setUserOrientation(orientation);

    emit sourceChanged();
}

bool ImageExtension::hasExif() const
{
    return m_ed ? true : false;
}

int ImageExtension::orientation() const
{
    return m_orientation;
}

void ImageExtension::setOrientation(int orientation)
{
    if (orientation >= TopUp && orientation <= RightUp && m_fileOrientation != orientation) {
        m_orientation = orientation;
        emit orientationChanged();
    }
}

int ImageExtension::userOrientation() const
{
    return m_userOrientation;
}

void ImageExtension::setUserOrientation(int userOrientation)
{
    userOrientation = userOrientation % 4;
    if (m_userOrientation!= userOrientation) {
        m_userOrientation = userOrientation;
        emit userOrientationChanged();
    }
}

void ImageExtension::saveInfo()
{
    saveIfChanged();
}

QString ImageExtension::fixSourceName(QString source) const
{
    if (source.startsWith("file://")) {
        return source.remove(0,7);
    }
    return source;
}

void ImageExtension::saveIfChanged()
{
    if (m_source.isEmpty() || m_orientation == m_userOrientation) {
        // no image or no change
        return;
    }

    QSettings settings(Organization, Application);
    settings.beginGroup(Orientations);
    if (m_userOrientation == m_fileOrientation)
        // remove override
        settings.remove(m_source);
    else {
        // override orientation in settings
        settings.setValue(m_source, m_userOrientation);
        settings.sync();
    }
}

int ImageExtension::exifOrientation() const
{
    // requires: m_fileOrientation is valid, between 0 and 3
    int mapOrientation[] = { 1, 8, 3, 6, 2, 5, 4, 7 };
    int index = m_userOrientation + (m_fileMirrored ? 4 : 0);
    return mapOrientation[index];
}
