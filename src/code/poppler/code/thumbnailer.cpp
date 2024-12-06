#include "thumbnailer.h"

#include <poppler/qt6/poppler-qt6.h>

#include <QImage>
#include <QUrl>
#include <QIcon>

Thumbnailer::Thumbnailer() : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{

}

QImage Thumbnailer::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "REQUESTED URL IS" << id << requestedSize;
    QImage result;

    auto document = Poppler::Document::load(QUrl::fromUserInput(id).toLocalFile());

    if(!document)
    {
        qWarning() << "Failed to get image from PDF document. Document can not be loaded" << id << requestedSize;
        return result;
    }

    if(document->isLocked())
    {
        result = QImage(":/img_assets/assets/emblem-locked.svg");
        // result = QIcon::fromTheme("love").pixmap(64).toImage();
        return result;
    }

    document->setRenderHint(Poppler::Document::Antialiasing, true);
    document->setRenderHint(Poppler::Document::TextAntialiasing, true);

           // If the requestedSize.width is 0, avoid Poppler rendering
           // FIXME: Actually it works correctly, but an error is anyway shown in the application output.

    auto page = document->page(0);

    if(!page)
    {
        qWarning() << "Failed to get image from PDF document. Page can not be loaded" << id << requestedSize;
        return result;
    }

    auto thumbnail = page->thumbnail();
    if(!thumbnail.isNull())
    {
        qDebug() << "Thumbnail retirved from page thumbnail";
        size->setHeight(thumbnail.height());
        size->setWidth(thumbnail.width());
        return thumbnail;
    }

    if (requestedSize.width() <= 0)
    {
        size->setHeight(400);
        size->setWidth(200);
    }else
    {
        size->setHeight(requestedSize.height());
        size->setWidth(requestedSize.width());
    }

    const qreal fakeDpiX = size->width() / page->pageSizeF().width() * 72.0;
    const qreal fakeDpiY = size->height() / page->pageSizeF().height() * 72.0;

           // Preserve aspect fit
    const qreal fakeDpi = std::min(fakeDpiX, fakeDpiY);

           // Render the page to QImage
    result = page->renderToImage(fakeDpi, fakeDpi);
    return result;
}


