/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDebug>
#include <QCryptographicHash>
#include <QSqlError>
#include <QSqlQuery>
#include <QProcess>

#include "browserlistmodel.h"

#define CHROMIUM_BROWSER_PATH "/usr/bin/meego-app-browser"

#define BROWSER_SERVICE "com.meego.browser.BrowserService"
#define BROWSER_SERVICE_PATH "/com/meego/browser/BrowserService"

static const QString THUMBNAIL_DIRNAME(QDir::home().path() + 
                                       QDir::separator() +
                                       ".config/internet-panel/thumbnails");

static const QString FAVICON_DIRNAME(QDir::homePath() + 
                                     QDir::separator() +
                                     ".config/internet-panel/favicons");

static const QString DB_FILENAME(QDir::homePath() + 
                                 QDir::separator() +
                                 ".config/internet-panel/chromium.db");

static inline QString computeChecksum(const QString& s)
{
    QByteArray ba = s.toUtf8();

    QByteArray md5 = QCryptographicHash::hash(ba, QCryptographicHash::Md5);

    QString csum("");

    for(int i = 0; i < md5.size(); i++)
        csum += QString("%1").arg((int)(md5[i]&0xFF), 2, 16, QLatin1Char('0'));

    return csum;
}

// Check whether the chromium browser is running by examing
// the DBus service exposed by chromium plugin:
// com.meego.browser.BrowserService
bool BrowserItemListModel::isBrowserRunning() const
{
	bool result = false;
	QDBusConnection bus = QDBusConnection::sessionBus();
	QDBusInterface iface("org.freedesktop.DBus", "/org/freedesktop/DBus",
			             "org.freedesktop.DBus", bus);

	QDBusReply<QStringList> reply = iface.call("ListNames");
	if(reply.isValid())
	{
		QStringList names = reply.value();
		if(names.contains(BROWSER_SERVICE)) result = true;
	}
	else 
		qDebug() << reply.error();
	
	return result;
}

BrowserItem::BrowserItem(qint64 id, const QString& url, const QString& title, 
		const QString& faviconUrl, QObject* parent) : QObject(parent),
	m_id(id), m_url(url), m_title(title), m_url_favicon(faviconUrl)
{
	// For favicon
	QString faviconPath;

	faviconPath.append(FAVICON_DIRNAME + QDir::separator());
	faviconPath.append(computeChecksum(faviconUrl));
	faviconPath.append(".png");
	if(QFile::exists(faviconPath))
	{
		m_faviconUri = faviconPath;
    m_faviconUri.prepend("file://");
	}

	// For thumbnail
	QString thumbPath;
	thumbPath.append(THUMBNAIL_DIRNAME + QDir::separator());
	thumbPath.append(computeChecksum(m_url));
	thumbPath.append(".jpg");

	if(QFile::exists(thumbPath))
	{
		m_thumbnailUri = thumbPath;
    m_thumbnailUri.prepend("file://");
	}
}

BrowserItemListModel::BrowserItemListModel(ModelType type, QObject* parent) : QAbstractListModel(parent),
m_type(type), m_sorttype(SortByDefault), m_limit(20), m_iface(NULL),m_browserIsLaunching(false)
{
	// Set Roles
    QHash<int, QByteArray> roles;

    roles.insert(BrowserItem::IdRole, "id");
    roles.insert(BrowserItem::UrlRole, "url");
    roles.insert(BrowserItem::TitleRole, "title");
    roles.insert(BrowserItem::IconUriRole, "faviconUri");
    roles.insert(BrowserItem::ThumbnailUriRole, "thumbnailUri");

    setRoleNames(roles);

	// Initialize DBus interface
	m_iface = new BrowserServiceInterface(
			BROWSER_SERVICE,
			BROWSER_SERVICE_PATH,
			QDBusConnection::sessionBus(), this);

	// Load data
    loadDataSource();

	// Listen the DBus for knowing the browser is launched
	connect(QDBusConnection::sessionBus().interface(),
			SIGNAL(serviceOwnerChanged(const QString&, const QString&, const QString&)),
			this, SLOT(dbusServiceChanged(const QString&, const QString&, const QString&)));

	connect(this, SIGNAL(browserLaunched()), this, SLOT(onBrowserLaunched()));
	connect(this, SIGNAL(browserExited()), this, SLOT(onBrowserExited()));

	// Connect the signal handler
	connect(m_iface, SIGNAL(faviconUpdated(const QString&)), this, SLOT(faviconUpdated(const QString&)));

	if(m_type == ListofRecentVisited)
	{
		connect(m_iface, SIGNAL(thumbnailUpdated(const QString&)), this, SLOT(thumbnailUpdated(const QString&)));
		connect(m_iface, SIGNAL(urlVisited(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(urlVisited(qint64, const QString&, const QString&, const QString&)));
		connect(m_iface, SIGNAL(urlRemoved(const QString&)), this, SLOT(urlRemoved(const QString&)));
	}

	if(m_type == ListofBookmarks)
	{
		connect(m_iface, SIGNAL(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)));
		connect(m_iface, SIGNAL(bookmarkRemoved(qint64)), this, SLOT(bookmarkRemoved(qint64)));
	}
	
}

BrowserItemListModel::~BrowserItemListModel()
{
    // clean db resource
    if(m_db.isValid())
    {
        m_db.close();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }
}

void BrowserItemListModel::clearData()
{
    QList<BrowserItem*>::iterator iter = m_items.begin();

    for(;iter != m_items.end(); iter++)
    {
        delete *iter;
    }

    m_items.clear();
}

bool BrowserItemListModel::loadDataSource()
{
    if(!m_db.isValid())
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE");

        m_db.setDatabaseName(DB_FILENAME);
        if(!m_db.open()){
            QSqlDatabase::removeDatabase(m_db.connectionName());
            qDebug() << "failed to open " << DB_FILENAME << ":" << m_db.lastError();
            return false;
        }
    }

    QString   stmt;

    // Prepare Sql statment
    if (m_type == ListofRecentVisited) // load recently visited page
    {
				stmt = QString("SELECT id, url, title, favicon_url, last_visit_time FROM urls "
                       "ORDER BY last_visit_time DESC LIMIT %1").arg(m_limit);

    } else if (m_type == ListofBookmarks) // load bookmark
    {
				stmt = QString("SELECT id, url, title, favicon_url, dateAdded FROM bookmarks "
                       "ORDER BY dateAdded DESC LIMIT %1").arg(m_limit);

    } else if (m_type == ListofTabs) // tab being opened in browser
    {
        // TODO:
        stmt = QString("SELECT tab_id, url, title, favicon_url FROM current_tabs LIMIT %1").arg(m_limit);
    } else
    {
        qDebug() << "Error: unknown page item type.";
        return false;
    }

    // Exec Sql stmt
    int count = 0;
    clearData();
    QSqlQuery query(stmt, m_db);
    while(query.next()) {

        if(m_limit > 0 && count++ > m_limit) break;

        int id = query.value(0).toInt();
        QString url = query.value(1).toString();
        QString title = query.value(2).toString();
        QString favicon_url = query.value(3).toString();

        BrowserItem *item = new BrowserItem(id, url, title, favicon_url);

        // prepend to list
        m_items.append(item);
    }
    query.finish();

    return true;
}

int BrowserItemListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant BrowserItemListModel::data(const QModelIndex& index, int role) const
{
    if(index.row() < 0 || index.row() > m_items.size())
        return QVariant();

    const BrowserItem* item = m_items[index.row()];

    if(role == BrowserItem::IdRole)
        return item->id();

    if(role == BrowserItem::UrlRole)
        return item->url();

    if(role == BrowserItem::TitleRole)
        return item->title();

    if(role == BrowserItem::IconUriRole)
        return item->faviconUri();

    if(role == BrowserItem::ThumbnailUriRole)
        return item->thumbnailUri();

    return QVariant();
}

void BrowserItemListModel::setType(ModelType type)
{
    if(m_type == type) return;
	if(m_type != ListofRecentVisited && m_type != ListofBookmarks && m_type != ListofTabs)
	{
		qDebug() << "Unknow item type: " << type;
		return;
	}

	// Disconnect previous signal connection
	if(m_type == ListofRecentVisited)
	{
		disconnect(m_iface, SIGNAL(urlVisited(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(urlVisited(qint64, const QString&, const QString&, const QString&)));
		disconnect(m_iface, SIGNAL(urlRemoved(const QString&)), this, SLOT(urlRemoved(const QString&)));
		disconnect(m_iface, SIGNAL(thumbnailUpdated(const QString&)), this, SLOT(thumbnailUpdated(const QString&)));
	}

	if(m_type == ListofBookmarks)
	{
		disconnect(m_iface, SIGNAL(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)));
		disconnect(m_iface, SIGNAL(bookmarkRemoved(qint64)), this, SLOT(bookmarkRemoved(qint64)));
	}

  m_type = type;

	// Reconnect signal connection according to type
	if(type == ListofRecentVisited){
		connect(m_iface, SIGNAL(urlVisited(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(urlVisited(qint64, const QString&, const QString&, const QString&)));
		connect(m_iface, SIGNAL(urlRemoved(const QString&)), this, SLOT(urlRemoved(const QString&)));
		connect(m_iface, SIGNAL(thumbnailUpdated(const QString&)), this, SLOT(thumbnailUpdated(const QString&)));				
	} else if(type == ListofBookmarks) {
		connect(m_iface, SIGNAL(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)), 
			this, SLOT(bookmarkUpdated(qint64, const QString&, const QString&, const QString&)));
		connect(m_iface, SIGNAL(bookmarkRemoved(qint64)), this, SLOT(bookmarkRemoved(qint64)));
	}

	//Load data model
	beginResetModel();
  loadDataSource();
	endResetModel();

	emit typeChanged(m_type); 
  return;
}

void BrowserItemListModel::setLimit(int max)
{
    if(max == m_limit) return;
	if(max < 0) 
	{
		qDebug() << "invalid limit value: " << max;
		return;
	}

    m_limit = max;

	beginResetModel();
    loadDataSource();
	endResetModel();

	emit limitChanged(max);
}

void BrowserItemListModel::setSortType(SortType type)
{
    if(m_sorttype == type) return;
    m_sorttype = type;
	//TODO
    return;
}

BrowserItem* BrowserItemListModel::getItem(qint64 id)
{
	QList<BrowserItem*>::iterator iter = m_items.begin();
	for(; iter != m_items.end() && (*iter)->id() != id; iter++);
	if(iter == m_items.end()) return NULL;
	else return *iter;
}

BrowserItem* BrowserItemListModel::getItem(const QString& url)
{
	int i;
	for(i = 0; i < m_items.length() && m_items[i]->url() != url; i++);
	if(i >= m_items.length()) return NULL;
	return m_items[i];
}

void BrowserItemListModel::viewItem(const QString& url)
{
	qDebug() << "view url: " << url;
	if(isBrowserRunning())
	{
		m_iface->viewItem(url);
		QProcess::startDetached(CHROMIUM_BROWSER_PATH);
	}
	else
	{
		if(!m_browserIsLaunching)
		{
			m_browserIsLaunching = QProcess::startDetached(CHROMIUM_BROWSER_PATH);
			if(!m_browserIsLaunching) m_urlToBeViewed.clear();
		}
		m_urlToBeViewed.append(url);
	}
	return;
}

// Remove a browser item from data model
bool BrowserItemListModel::internalRemoveItem(int id)
{
	BrowserItem* item = getItem(id);
	if(item == NULL) return false;
	
	int i = m_items.indexOf(item);
	beginRemoveRows(QModelIndex(), i, i);
	m_items.removeOne(item);
	delete item;
	endRemoveRows();
	
	// Remove it from backend database
	QString stmt;
	QSqlQuery query;

	if(m_type == ListofRecentVisited)
	{
		stmt = QString("DELETE FROM urls WHERE id=%1").arg(id);
	} 
	if(m_type == ListofBookmarks)
	{
		stmt = QString("DELETE FROM bookmarks WHERE id=%1").arg(id);
	}
	query.exec(stmt);

	query.finish();

	loadDataSource();

	if(m_items.length() == m_limit)
	{
		// The last one is a new one which was selected from backend data store
		beginInsertRows(QModelIndex(), m_items.length() - 1, m_items.length() - 1);
		endInsertRows();
	}

	return true;
}

//
// Remove a browser item with the given id
//
void BrowserItemListModel::destroyItem(int id)
{

	QString sid = QString("%1").arg(id);
	BrowserItem* item = getItem(id);
	if(item == NULL) return;

	if(!isBrowserRunning())
	{
		qDebug() << "Browser is not running";
		if(m_type == ListofRecentVisited)
			m_vistedpageToBeDeleted << item->url();
		if(m_type == ListofBookmarks)
			m_bookmarksToBeDeleted << sid;
	}

	else
	{
		if(m_type == ListofRecentVisited)
			m_iface->removeUrl(item->url());

		if(m_type == ListofBookmarks)
			m_iface->removeBookmark(sid);
	}
	
	internalRemoveItem(id);

	return;
}

void BrowserItemListModel::clearAllItems()
{
	QString stmt;
	QSqlQuery query;

	// Clear item contained in list model
	beginResetModel();
	m_items.clear();
	endResetModel();

	if(m_type == ListofRecentVisited) {
			stmt = QString("DELETE FROM urls");
	} else if(m_type == ListofBookmarks) {
			stmt = QString("DELETE FROM bookmarks");
	} else {
				qDebug() << "unknown type";
				return;
	}
	// Clear data in database via executing sql stmt
	query.exec(stmt);
	query.finish();
	return;
}

void BrowserItemListModel::bookmarkRemoved(qint64 id)
{
	qDebug() << "bookmarkRemoved: " << id;

	if(m_type != ListofBookmarks) return;

	BrowserItem* item = getItem(id);

	if(!item) return;

	int idx = m_items.indexOf(item);

	beginRemoveRows(QModelIndex(), idx, idx);
	m_items.removeOne(item);
	delete item;
	endRemoveRows();

	loadDataSource();

	if(m_items.length() == m_limit)
	{
		beginInsertRows(QModelIndex(), m_items.length() - 1, m_items.length() - 1);
		endInsertRows();
	}
}

void BrowserItemListModel::bookmarkUpdated(qint64 id, const QString &url, const QString &title, const QString &faviconUrl)
{
	qDebug() << "################## bookmarkUpdate: " << url;
	if(m_type != ListofBookmarks) return;
	
	BrowserItem* item = getItem(id);
	if(item) // already exisits
	{
		int idx = m_items.indexOf(item);
		if(idx == 0) return;

		beginMoveRows(QModelIndex(), idx, idx, QModelIndex(), 0);
		m_items.removeOne(item);
		m_items.prepend(item);
		endMoveRows();
	}
	else
	{
		if(m_items.length() >= m_limit)
		{
      item = m_items[m_items.length()-1];
			beginRemoveRows(QModelIndex(), m_items.length() - 1, m_items.length() - 1);
			m_items.removeOne(item);
			delete item;
			endRemoveRows();
		}

		beginInsertRows(QModelIndex(), 0, 0);
		item = new BrowserItem(id, url, title, faviconUrl);
		m_items.prepend(item);
		endInsertRows();
	}
	
	return;
}

void BrowserItemListModel::faviconUpdated(const QString &url)
{
	qDebug() << "faviconUpdated: " << url;

	for(int i = 0; i < m_items.length(); i++)
	{
		if(m_items[i]->m_url_favicon == url)
		{
			
			//if(!m_items[i]->m_faviconUri.isEmpty()) return;

			QString& m_faviconUri = m_items[i]->m_faviconUri;	
			m_faviconUri.clear();
			m_faviconUri.append("file:/");
			m_faviconUri.append(FAVICON_DIRNAME + QDir::separator());
			m_faviconUri.append(computeChecksum(url));
			m_faviconUri.append(".png");

			//emit m_items[i]->faviconUriChanged(m_faviconUri);
			QModelIndex idx = index(m_items.indexOf(m_items[i]));
			emit dataChanged(idx, idx);
		}
	}
	beginResetModel();
	reset();
	endResetModel();
}

void BrowserItemListModel::thumbnailUpdated(const QString &url)
{
	qDebug() << "thumbnailUpdated: " << url;

	BrowserItem* item = getItem(url);

	if(item)
	{
		QString& m_thumbnailUri = item->m_thumbnailUri;

		m_thumbnailUri.clear();
		m_thumbnailUri.append("file:/");
		m_thumbnailUri.append(THUMBNAIL_DIRNAME + QDir::separator());
		m_thumbnailUri.append(computeChecksum(url));
		m_thumbnailUri.append(".jpg");
		
		QModelIndex idx = index(m_items.indexOf(item));
		emit dataChanged(idx, idx);
		beginResetModel();
		reset();
		endResetModel();
	}
}

void BrowserItemListModel::urlRemoved(const QString &url)
{
	qDebug() << "urlRemoved: " << url;
	if(m_type != ListofRecentVisited) return;

	BrowserItem* item = getItem(url);
	if(item == NULL) return;

	int idx = m_items.indexOf(item);
	beginRemoveRows(QModelIndex(), idx, idx);
	m_items.removeOne(item);
	delete item;
	endRemoveRows();

	loadDataSource();

	if(m_items.length() == m_limit)
	{
		beginInsertRows(QModelIndex(), m_items.length() - 1, m_items.length() - 1);
		endInsertRows();
	}
}

void BrowserItemListModel::urlVisited(qint64 id, const QString &url, const QString &title, const QString &faviconUrl)
{
	qDebug() << "urlVisited: " << url;

	if(m_type != ListofRecentVisited) return;
	
	BrowserItem* item = getItem(id);

	if(item) // The visited URL already exists in list
	{
		int idx = m_items.indexOf(item);
		if(idx == 0) return;
		beginMoveRows(QModelIndex(), idx, idx, QModelIndex(), 0); 
		m_items.removeOne(item);
		m_items.prepend(item);
		endMoveRows();
	}
	else
	{
		// A new URL visited
		int length = m_items.length();
		if(length >= m_limit)
		{
      item = m_items[length - 1];
			beginRemoveRows(QModelIndex(), length - 1, length - 1);
			m_items.removeOne(item);
			delete item;
			endRemoveRows();
		}

		qDebug() << "prepend a new item ";
		// Prepend the new one;
		beginInsertRows(QModelIndex(), 0, 0);
		BrowserItem* newitem = new BrowserItem(id, url, title, faviconUrl);
		m_items.prepend(newitem);
		endInsertRows();
		qDebug() << "end of prepending";
	}
	
	return;
}

void BrowserItemListModel::dbusServiceChanged(const QString& name, const QString& old_owner, const QString& new_owner)
{
	qDebug() << "dbusServiceChanged: " << name << "," << new_owner << "," << old_owner;
	if(name == BROWSER_SERVICE && new_owner != "") emit browserLaunched();
	if(name == BROWSER_SERVICE && new_owner == "") emit browserExited();
}

void BrowserItemListModel::destroyItemsByBrowser()
{
	qDebug() << "destroyItemsByBrowser";

	QList<QString>::const_iterator iter = m_bookmarksToBeDeleted.begin();

	// Remove the bookmarks to be deleted
	for(; iter != m_bookmarksToBeDeleted.end(); iter++)
	{
		m_iface->removeBookmark(*iter);
	}

	m_bookmarksToBeDeleted.clear();

	// Remove the visited page to be deleted
	QList<QString>::const_iterator iter2 = m_vistedpageToBeDeleted.begin();
	for(; iter2 != m_vistedpageToBeDeleted.end(); iter2++)
	{
		m_iface->removeUrl(*iter2);
	}
	m_vistedpageToBeDeleted.clear();
}

void BrowserItemListModel::viewItemsByBrowser()
{
	qDebug() << "viewItemByBrowser " << m_urlToBeViewed;

	int len = m_urlToBeViewed.length();
	for(int i = 0; i < len; i++)
	{
		m_iface->viewItem(m_urlToBeViewed[i]);
	}
	m_urlToBeViewed.clear();
	return;
}

void BrowserItemListModel::onBrowserLaunched()
{
	destroyItemsByBrowser();
	viewItemsByBrowser();
}

void BrowserItemListModel::onBrowserExited()
{
	m_browserIsLaunching = false;
	m_urlToBeViewed.clear();
}
