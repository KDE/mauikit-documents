// SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QQmlEngine>
#include <QResource>
#include <QFontDatabase>

#include "plugin.h"

//poppler stuff
#include <poppler/code/pdfdocument.h>
#include <poppler/code/thumbnailer.h>

//cb stuff
#include <cbz/code/ArchiveBookModel.h>
#include <cbz/code/BookListModel.h>
#include <cbz/code/BookModel.h>
#include <cbz/code/ComicCoverImageProvider.h>
#include <cbz/code/FolderBookModel.h>
#include <cbz/code/PreviewImageProvider.h>

#include <cbz/code/FilterProxy.h>
#include <cbz/code/PropertyContainer.h>

#include <cbz/code/acbf/AcbfBinary.h>
#include <cbz/code/acbf/AcbfReference.h>
#include <cbz/code/acbf/AcbfStyle.h>
#include <cbz/code/acbf/AcbfIdentifiedObjectModel.h>

void DocumentsPlugin::registerTypes(const char *uri)
{
#if defined(Q_OS_ANDROID)
    QResource::registerResource(QStringLiteral("assets:/android_rcc_bundle.rcc"));
    QResource::registerResource(QStringLiteral("fonts.qrc"), "fonts");

    if(QFile::exists(":/fonts/Courier/Courier.ttf"))
    {
        qDebug() << "Courier font exists";
    }
    QFontDatabase::addApplicationFont(":/fonts/Courier/Courier.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Courier/Courier Bold Oblique.otf");
    QFontDatabase::addApplicationFont(":/fonts/Helvetica-Font/Helvetica-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Helvetica-Font/Helvetica-BoldOblique.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Helvetica-Font/Helvetica-Oblique.ttf");
    QFontDatabase::addApplicationFont(":/fonts/times/TIMESR.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Helvetica-Font/Helvetica.ttf");
    #endif

    //Poppler stuff
    qmlRegisterType<PdfDocument>(uri, 1, 0, "Document");
    qmlRegisterType(resolveFileUrl(QStringLiteral("poppler/PDFViewer.qml")), uri, 1, 0, "PDFViewer");
    qmlRegisterType(resolveFileUrl(QStringLiteral("FileDialog.qml")), uri, 1, 0, "FileDialog");

    //CB stuff
    qmlRegisterType<CategoryEntriesModel>(uri, 1, 0, "CategoryEntriesModel");
       qmlRegisterType<BookListModel>(uri, 1, 0, "BookListModel");
       qmlRegisterType<BookModel>(uri, 1, 0, "BookModel");
       qmlRegisterType<ArchiveBookModel>(uri, 1, 0, "ArchiveBookModel");
       qmlRegisterType<FolderBookModel>(uri, 1, 0, "FolderBookModel");
       qmlRegisterType<PropertyContainer>(uri, 1, 0, "PropertyContainer");
       qmlRegisterType<FilterProxy>(uri, 1, 0, "FilterProxy");

       qmlRegisterUncreatableType<AdvancedComicBookFormat::Reference>(uri, 1, 0, "Reference", "Don't attempt to create ACBF types directly, use the convenience functions on their container types for creating them");
        qmlRegisterUncreatableType<AdvancedComicBookFormat::Binary>(uri, 1, 0, "Binary", "Don't attempt to create ACBF types directly, use the convenience functions on their container types for creating them");
        qmlRegisterUncreatableType<AdvancedComicBookFormat::Style>(uri, 1, 0, "Style", "Don't attempt to create ACBF types directly, use the convenience functions on their container types for creating them");
        qmlRegisterType<AdvancedComicBookFormat::IdentifiedObjectModel>(uri, 1, 0, "IdentifiedObjectModel");

}

void DocumentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    /** IMAGE PROVIDERS **/
    engine->addImageProvider("preview", new Thumbnailer());


//    engine->addImageProvider("preview", new PreviewImageProvider());
    engine->addImageProvider("comiccover", new ComicCoverImageProvider());

}
