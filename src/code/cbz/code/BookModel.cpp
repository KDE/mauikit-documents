/*
 * Copyright (C) 2015 Dan Leinir Turthra Jensen <admin@leinir.dk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "BookModel.h"

#include <QDebug>
#include <AcbfDocument.h>

#ifdef KFILEMETADATA_FOUND
#include <KFileMetaData/UserMetaData>
#endif

struct BookPage {
    BookPage() {}
    QString url;
    QString title;
};

class BookModel::Private {
public:
    Private()
        : currentPage(0)
        , acbfData(nullptr)
        , processing(false)
    {}
    QString filename;
    QString author;
    QString publisher;
    QString title;
    QList<BookPage*> entries;
    int currentPage;
    AdvancedComicBookFormat::Document* acbfData;
    bool processing;
    QString processingDescription;
};

BookModel::BookModel(QObject* parent)
    : QAbstractListModel(parent)
    , d(new Private)
{
}

BookModel::~BookModel()
{
    delete d;
}

QHash<int, QByteArray> BookModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[TitleRole] = "title";
    return roles;
}

QVariant BookModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if(index.isValid() && index.row() > -1 && index.row() < d->entries.count())
    {
        const BookPage* entry = d->entries[index.row()];
        switch(role)
        {
            case UrlRole:
                result.setValue(entry->url);
                break;
            case TitleRole:
                result.setValue(entry->title);
                break;
            default:
                result.setValue(QString("Unknown role"));
                break;
        }
    }
    return result;
}

int BookModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;
    return d->entries.count();
}

void BookModel::addPage(QString url, QString title)
{
    BookPage* page = new BookPage();
    page->url = url;
    page->title = title;

    beginInsertRows(QModelIndex(), d->entries.count(), d->entries.count());
    d->entries.append(page);
    Q_EMIT pageCountChanged();
    endInsertRows();
}

void BookModel::removePage(int pageNumber)
{
    QModelIndex index  = createIndex(pageNumber, 0);
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    d->entries.removeAt(pageNumber);
    Q_EMIT pageCountChanged();
    endRemoveRows();
}

void BookModel::clearPages()
{
    beginResetModel();
    qDeleteAll(d->entries);
    d->entries.clear();
    Q_EMIT pageCountChanged();
    endResetModel();
}

QString BookModel::filename() const
{
    return d->filename;
}

void BookModel::setFilename(QString newFilename)
{
    d->filename = newFilename;
    d->title = newFilename.split('/').last().left(newFilename.lastIndexOf('.'));
    Q_EMIT filenameChanged();
    Q_EMIT titleChanged();
}

QString BookModel::author() const
{
    return d->author;
}

void BookModel::setAuthor(QString newAuthor)
{
    d->author = newAuthor;
    Q_EMIT authorChanged();
}

QString BookModel::publisher() const
{
    return d->publisher;
}

void BookModel::setPublisher(QString newPublisher)
{
    d->publisher = newPublisher;
    Q_EMIT publisherChanged();
}

QString BookModel::title() const
{
    return d->title;
}

void BookModel::setTitle(QString newTitle)
{
    d->title = newTitle;
    Q_EMIT titleChanged();
}

int BookModel::pageCount() const
{
    return d->entries.count();
}

int BookModel::currentPage() const
{
    return d->currentPage;
}

void BookModel::setCurrentPage(int newCurrentPage, bool updateFilesystem)
{
//     qDebug()<< Q_FUNC_INFO << d->filename << newCurrentPage << updateFilesystem;
#ifdef KFILEMETADATA_FOUND
    if(updateFilesystem)
    {
        KFileMetaData::UserMetaData data(d->filename);
        data.setAttribute("peruse.currentPage", QString::number(newCurrentPage));
    }
#endif

    d->currentPage = newCurrentPage;
    Q_EMIT currentPageChanged();
}

QObject * BookModel::acbfData() const
{
    return d->acbfData;
}

void BookModel::setAcbfData(QObject* obj)
{
    d->acbfData = qobject_cast<AdvancedComicBookFormat::Document*>(obj);
    Q_EMIT acbfDataChanged();
}

bool BookModel::processing() const
{
    return d->processing;
}

void BookModel::setProcessing(bool processing)
{
    d->processing = processing;
    Q_EMIT processingChanged();
}

QString BookModel::processingDescription() const
{
    return d->processingDescription;
}

void BookModel::setProcessingDescription ( const QString& description )
{
    d->processingDescription = description;
    qDebug()<< description;
    Q_EMIT processingDescriptionChanged();
}

void BookModel::swapPages(int swapThisIndex, int withThisIndex)
{
    if(swapThisIndex > -1 && withThisIndex > -1 && swapThisIndex < d->entries.count() && withThisIndex < d->entries.count()) {
        QModelIndex firstIndex = createIndex(swapThisIndex, 0);
        QModelIndex secondIndex = createIndex(withThisIndex, 0);
        d->entries.swapItemsAt(swapThisIndex, withThisIndex);
        dataChanged(firstIndex, secondIndex);
    }
}
