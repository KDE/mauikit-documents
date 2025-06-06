/*
 * Copyright (C) 2013-2015 Canonical, Ltd.
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
 * Author: Anthony Granger <grangeranthony@gmail.com>
 *         Stefano Verzegnassi <stefano92.100@gmail.com>
 */

#pragma once

#include <QAbstractListModel>

#include <poppler/qt6/poppler-qt6.h>

#include "pdfitem.h"
#include "pdftocmodel.h"
#include <QUrl>

// typedef std::list<std::unique_ptr<Poppler::Page>> PdfPagesList;

class PdfDocument : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PdfDocument)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(int pages READ pageCount NOTIFY pagesCountChanged)
    Q_PROPERTY(int providersNumber READ providersNumber NOTIFY providersNumberChanged)
    Q_PROPERTY(QObject* tocModel READ tocModel NOTIFY tocModelChanged)
    Q_PROPERTY(bool isLocked READ isLocked NOTIFY isLockedChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(QString id READ id CONSTANT FINAL)

public:
    enum Roles {
        WidthRole = Qt::UserRole + 1,
        HeightRole,
        UrlRole,
        LinksRole
    };

    explicit PdfDocument(QAbstractListModel *parent = nullptr);
    virtual ~PdfDocument();

    QUrl path() const { return m_path; }
    void setPath(QUrl &pathName);

    int pageCount() const;
    int providersNumber() const { return m_providersNumber; }

    QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override final;

    Q_INVOKABLE QDateTime getDocumentDate(QString data);
    Q_INVOKABLE QString getDocumentInfo(QString data) const;

    QObject *tocModel() const { return m_tocModel; }

    QString title() const;

    bool isLocked() const;

    bool isValid() const;

    QString id() const;

Q_SIGNALS:
    void pathChanged();
    void error(const QString& errorMessage);
    void pagesLoaded();
    void providersNumberChanged();
    void tocModelChanged();
    void pagesCountChanged();
    void documentLocked();
    void titleChanged();

    void isLockedChanged();

    void isValidChanged();

private Q_SLOTS:
    // void _q_populate(PdfPagesList pagesList);
    
public Q_SLOTS:
    void unlock(const QString &ownerPassword, const QString &password);
    QVariantList search(int page, const QString& text, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    QString getText(const QRectF &rect,const QSize &pageSize, int page);
    
private:
    QUrl m_path;
    QString m_id; //id of this document for the provider
    int m_providersNumber;
    int pages;

    bool loadDocument(const QString &pathName, const QString &password = QString(), const QString &userPassword = QString());
    void loadProvider();
    bool loadPages();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Poppler::Document *m_document;
#else
    std::unique_ptr<Poppler::Document> m_document;
#endif
    QList<PdfItem> m_pages;
    PdfTocModel* m_tocModel;
    bool m_isValid = false;
};
