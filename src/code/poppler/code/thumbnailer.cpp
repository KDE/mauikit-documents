#include "thumbnailer.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <poppler/qt5/poppler-qt5.h>
#else
#include <poppler/qt6/poppler-qt6.h>
#endif

#include <QImage>
#include <QUrl>

Thumbnailer::Thumbnailer() : QQuickImageProvider(QQuickImageProvider::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{

}

QImage Thumbnailer::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage result;

    auto document = Poppler::Document::load(QUrl::fromUserInput(id).toLocalFile());

    if(!document)
    {
        return result;
    }

    if(document->isLocked())
    {
        result = QImage("qrc://img_assets/assets/emblem-locked.svg");
        return result;
    }

    document->setRenderHint(Poppler::Document::Antialiasing, true);
    document->setRenderHint(Poppler::Document::TextAntialiasing, true);

    // If the requestedSize.width is 0, avoid Poppler rendering
    // FIXME: Actually it works correctly, but an error is anyway shown in the application output.
    if (requestedSize.width() > 0)
    {
        auto page = document->page(0);

        if(!page)
        {
            return result;
        }

        size->setHeight(requestedSize.height());
        size->setWidth(requestedSize.width());

        const qreal fakeDpiX = requestedSize.width() / page->pageSizeF().width() * 72.0;
        const qreal fakeDpiY = requestedSize.height() / page->pageSizeF().height() * 72.0;

        // Preserve aspect fit
        const qreal fakeDpi = std::min(fakeDpiX, fakeDpiY);

        // Render the page to QImage
        result = page->renderToImage(fakeDpi, fakeDpi);

        return result;
    }

    // Requested size is 0, so return a null image.
    return QImage();
}


