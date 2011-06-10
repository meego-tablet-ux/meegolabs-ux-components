/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "librarymodel.h"

#include <QtDeclarative/qdeclarative.h>
#include <QFile>
#include <QXmlStreamReader>

Book::Book(QObject* parent)
    :QObject(parent)

{
}

bool Book::deserialize( QXmlStreamReader* const xmlin )
{
    while (xmlin->readNextStartElement())
    {
        if (xmlin->name() == "title")
        {
            m_title= xmlin->readElementText();
        }
        else if (xmlin->name() == "author")
        {
            m_author= xmlin->readElementText();
        }
        else if (xmlin->name() == "cover")
        {
            m_cover= xmlin->readElementText();
        }
    }

    return false;
}


LibraryModel::LibraryModel(QObject *parent) :
        QAbstractItemModel(parent)
{
}

bool LibraryModel::setFile( const QString &fileName )
{
    qDebug() << "YES";
    QFile file( fileName );
    if ( file.open( QIODevice::ReadOnly) )
    {
        m_xml.setDevice(&file);
        deserialize();

        qDebug() << "FILE IS READ";
        return m_xml.error();
    }
    else
    {
        qDebug() << "FILE NONE EXISTENT";
        return false;
    }
}


bool LibraryModel::deserialize()
{
    while ( m_xml.readNextStartElement() )
    {
        if( m_xml.name() != "library")
            readCategory();
    }

    return m_xml.error();

}

bool LibraryModel::readCategory()
{

    QString category = m_xml.name().toString();
    int m_categoriesIndex= m_elements.count();
    m_categories.append(category);
    m_parents.insert(category ,m_categoriesIndex );

    while ( m_xml.readNextStartElement() )
    {
        if ( m_xml.name() == "book" )
        {
            Book* aBook= new Book;
            aBook->deserialize(&m_xml);
            m_elements << aBook;           
        }
    }


    return m_xml.error();
}


QModelIndex LibraryModel::index( int row, int column, const QModelIndex & parent ) const
{

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
    {
        return createIndex(row, column, -99);
    }
    else
    {
        return createIndex(row, column, parent.row() );
    }
}

QModelIndex LibraryModel::parent(const QModelIndex &child) const
{

    if ( child.internalId() == -99 )
        return QModelIndex();
    else
        return createIndex( child.internalId(), 0, -99 );

}

int LibraryModel::rowCount(const QModelIndex &parent ) const
{
    if (parent.column() > 0)
            return 0;


    if ( !parent.isValid() )
    {
        return m_categories.count();
    }
    else if ( parent.internalId() == -99 )
    {
        if ( parent.row() + 1 <  m_categories.count() )
        {
            return ( m_parents.value( m_categories.at( parent.row() + 1 ) ) -
                     m_parents.value( m_categories.at( parent.row() ) ) );

        }
        else
            return ( m_elements.count() -
                     m_parents.value( m_categories.at( parent.row() ) ) );

    }
    else
        return 0;


}

int LibraryModel::columnCount(const QModelIndex &parent ) const
{  
    Q_UNUSED(parent);
    return 3;
}

QVariant LibraryModel::data(const QModelIndex &index, int role ) const
{

       if (!index.isValid())
           return QVariant();

       if (role != Qt::DisplayRole)
           return QVariant();    

       if ( !index.parent().isValid() )
       {
           if (  index.column() == 0 )
                return m_categories.at(index.row());
           else
               return QVariant();


       }
       else if ( index.column() == 0 )
       {
            return m_elements.at( m_parents.value( m_categories.at( index.parent().row() ) )
                                 + index.row() )->getTitle();
       }
       else if ( index.column() == 1 )
       {
           return m_elements.at( m_parents.value( m_categories.at( index.parent().row() ) )
                                + index.row() )->getAuthor();
       }
       else if ( index.column() == 2 )
       {
           return m_elements.at( m_parents.value( m_categories.at( index.parent().row() ) )
                                + index.row() )->getCover();
       }


       return QVariant();
}


QML_DECLARE_TYPE(Book);
QML_DECLARE_TYPE(LibraryModel);
