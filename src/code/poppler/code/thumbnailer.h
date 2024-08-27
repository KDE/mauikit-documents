#pragma once

#include <QQuickImageProvider>

class Thumbnailer : public QQuickImageProvider
{
public:
    Thumbnailer();
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize) override;

};
