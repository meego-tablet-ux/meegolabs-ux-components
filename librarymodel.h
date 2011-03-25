/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QHash>
#include <QDebug>
#include <QtDeclarative>
#include <QList>
#include <QStringList>
#include <QXmlStreamReader>


class Book:
        public QObject
{
    Q_OBJECT

    Q_PROPERTY (QString title READ getTitle)
    Q_PROPERTY (QString author READ getAuthor)
    Q_PROPERTY (QString cover READ getCover)

    friend class LibraryModel;

public:
     Book(QObject* parent=0);

     QString getTitle() const { return m_title;}
     QString getAuthor() const { return m_author;}
     QString getCover() const { return m_cover;}


signals:

public slots:

protected:
    bool deserialize( QXmlStreamReader* const xmlin );

private:
    QString m_title;
    QString m_author;
    QString m_cover;
};

class LibraryModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY (QString xmlFile READ file WRITE setFile)
    Q_PROPERTY (QStringList categories READ categories)
    Q_PROPERTY (QDeclarativeListProperty<Book> books READ books)


public:
    explicit LibraryModel(QObject *parent = 0);
    QString file() const { return m_fileName; }
    bool setFile( const QString &fileName );

    QDeclarativeListProperty<Book> books() {  return QDeclarativeListProperty<Book>(this, m_elements); }
    QStringList categories() const {
        return m_categories; }


signals:

public slots:

protected:
    QModelIndex index( int row, int column, const QModelIndex & parent ) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    bool deserialize();
    bool readCategory();

    QString      m_fileName;
    QList<Book*> m_elements;

    QStringList    m_categories;
    QHash<QString, int> m_parents;

    QXmlStreamReader m_xml;
    Q_DISABLE_COPY(LibraryModel)
};

#endif // LIBRARYMODEL_H
