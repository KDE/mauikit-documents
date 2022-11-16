// SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QQmlEngine>
#include <QResource>
#include <QFontDatabase>

#include "plugin.h"

#include <poppler/code/pdfdocument.h>
#include <poppler/code/thumbnailer.h>

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

}

void DocumentsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    /** IMAGE PROVIDERS **/
    engine->addImageProvider("preview", new Thumbnailer());
}
