/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H

#include <QtDeclarative>
#include <QObject>
#include <QStringList>

class QFileSystemWatcher;
class MGConfItem;

class BackgroundModel : public QAbstractListModel
{
    enum Role {
        Uri = Qt::UserRole + 1,
        Active = Qt::UserRole + 2
   };

    Q_OBJECT;
    Q_PROPERTY(QString activeWallpaper READ activeWallpaper WRITE setActiveWallpaper NOTIFY activeWallpaperChanged);
    Q_PROPERTY(QString path READ path WRITE setPath);

public:
    explicit BackgroundModel(QObject* parent=0);

    QString activeWallpaper() const {
        return selected;
    }
    void setActiveWallpaper(QString file);

    QString path() const {
        return m_path;
    }
    void setPath(QString path) {
        m_path = path;
        if (path.startsWith(QString("~/")))
        {
            m_path.remove(0, 1);
            m_path.prepend(QDir::homePath());
        }
        if (!QDir(m_path).exists())
        {
            QDir().mkpath(m_path);
        }
        filesystemUpdate();
    }

signals:
    void modelReset();
    void activeWallpaperChanged();

public slots:
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void select(int index);
    QString copyImageToBackgrounds(QString path);
    void themeChanged();

private slots:
    void filesystemUpdate(QString file = QString());
    void backgroundChanged();

private:
    QFileSystemWatcher *watcher;
    QStringList wallpapers;
    QString selected;
    QString m_path;
    MGConfItem *backgroundGConfItem;
    MGConfItem *themeGConfItem;
};

#endif // BACKGROUNDMODEL_H
