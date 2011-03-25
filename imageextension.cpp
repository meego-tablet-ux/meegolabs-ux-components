/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "imageextension.h"
#include <QFile>
ImageExtension::ImageExtension(QObject *parent) :
        QObject(parent) ,
        m_source(QString()),
        m_ed(0)
{
}

ImageExtension::~ImageExtension()
{
    delete m_ed;
}
void ImageExtension::setSource(const QString source)
{
    QString fixed_source = fixSourceName(source);
    if (m_source == fixed_source) {
        return;
    }
    m_source = fixed_source;

    //if (m_ed != 0) {
    delete m_ed;
    delete m_image;
    //}
    m_ed = exif_data_new_from_file(m_source.toUtf8().constData());
    m_image = new QImage(source);
    m_usrOrientation = orientation();
    emit orientationChanged();
    emit hasExifChanged();
}

int ImageExtension::orientation() const
{
    if (!m_ed) {
        return 1;
    }
    ExifEntry *entry;
    ExifByteOrder byte_order;
    byte_order = exif_data_get_byte_order (m_ed);

    if ((entry = exif_content_get_entry(m_ed->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION)))
    {
        return exif_get_short (entry->data, byte_order);

    }
    return 1;
}

bool ImageExtension::hasExif() const
{
    return (m_ed != 0);
}

QString ImageExtension::fixSourceName(QString source) const
{
    if (source.startsWith("file://")) {
        return source.remove(0,7);
    }
    return source;
}

void ImageExtension::saveIfChanged() const
{
    if (!m_image) {
        // no data to save, return;
        return;
    }
    if (usrOrientation() != orientation()) {
        if (hasExif()) {
            // change the corresponding EXIF tag
            ExifEntry *entry;
            ExifByteOrder byte_order;
            byte_order = exif_data_get_byte_order (m_ed);

            if (entry= exif_content_get_entry(m_ed->ifd[EXIF_IFD_0], EXIF_TAG_ORIENTATION))
            {
                exif_set_short(entry->data, byte_order, usrOrientation());
            }else {
                // does not exist the ORIENTATION  tag, create a new one.
                entry = exif_entry_new();
                entry->tag = EXIF_TAG_ORIENTATION;
                exif_content_add_entry(m_ed->ifd[EXIF_IFD_0], entry);
                exif_entry_initialize(entry, EXIF_TAG_ORIENTATION);
                exif_set_short(entry->data, byte_order,usrOrientation());
                exif_entry_unref(entry);
            }
            unsigned char* exif_data;
            unsigned int exif_data_len;
            QFile file(m_source);
            quint64 size = file.size();
            quint64 startPt = -1;
            quint64 endPt = -1;
            unsigned exif_tag_size = 0;

            exif_data_save_data(m_ed, &exif_data, &exif_data_len);

            if (!file.open(QIODevice::ReadOnly)) {
                return;
            }
            uchar * content = file.map(0, size);
            if (content == 0) {
                return;
            }

            for (startPt = 2; startPt < size; startPt += 2) {
                if (content[startPt] == 0xff && content[startPt +1] == 0xe1) {
                    // found the start of EXIF tag
                    startPt += 2;
                    break;
                }
            }
            // if didn't find the exif tag
            // should not happen
            if (startPt == -1) {
                return;
            }

            // if the size is incorrect ...
            if (size < startPt + 2) {
                return;
            }
             exif_tag_size = (content[startPt] << 8) + content[startPt +1] ;
             endPt = startPt + exif_tag_size;
            // open a temp file
            QFile tempImage("/tmp/image.tmp");
            if (!tempImage.open(QIODevice::WriteOnly)) {
                file.unmap(content);
                file.close();
                return;
            }

            QDataStream out(&tempImage);
            out.writeRawData((const char*)content, startPt);
            tempImage.putChar((exif_data_len + 2) >> 8);
            tempImage.putChar((exif_data_len + 2) & 0xff);
            out.writeRawData((const char*)exif_data, exif_data_len);
            out.writeRawData((const char*)(content + endPt), size - endPt);
            tempImage.close();
            file.unmap(content);
            file.close();
            QFile::remove(source());
            QFile::copy("/tmp/image.tmp", source());
        }else {
            // does not have exif tag;
            QImage image(source());
            if (!image.isNull()) {
                QTransform t;
                switch (m_usrOrientation) {
                case 1:
                   return;
                case 8:
                   t.rotate(270);
                   break;
                case 3:
                   t.rotate(180);
                   break;
                case 6:
                   t.rotate(90);
                   break;
                default:
                   return;
                }
                 image = image.transformed(t);
                 image.save(source());
                 return;
            }
        }
    }else {
        // no change
        return;
    }
}

void ImageExtension::saveInfo()
{
    saveIfChanged();
}
