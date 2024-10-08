/*
 * Copyright (C) 2015
 *          Stefano Verzegnassi <verzegnassi.stefano@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QAbstractListModel>

#include <poppler/qt6/poppler-qt6.h>

class TocEntry
{
public:
    QString title;
    int pageIndex = 0;
    int level = 0;
};

class PdfTocModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PdfTocModel)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        TitleRole = Qt::UserRole + 1,
        PageIndexRole,
        LevelRole
    };

    explicit PdfTocModel(QAbstractListModel *parent = nullptr);
    virtual ~PdfTocModel();

    void setDocument(Poppler::Document *document);

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QVariantMap get(int index) const;

Q_SIGNALS:
    void documentChanged();
    void countChanged();

private Q_SLOTS:
    void fillModel();

private:
    Poppler::Document* m_document;

    QList<TocEntry> m_entries;

    void recursiveGetEntries(QVector<Poppler::OutlineItem> node, int nodeLevel);

};

