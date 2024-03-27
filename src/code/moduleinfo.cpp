#include "moduleinfo.h"
#include "../documents_version.h"
#include <KLocalizedString>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <poppler/poppler-config.h>
#else
#include <poppler/qt6/poppler-version.h>
#endif

QString MauiKitDocuments::versionString()
{
    return QStringLiteral(Documents_VERSION_STRING);
}

QString MauiKitDocuments::buildVersion()
{
    return GIT_BRANCH+QStringLiteral("/")+GIT_COMMIT_HASH;
}

KAboutComponent MauiKitDocuments::aboutData()
{
    return KAboutComponent(QStringLiteral("MauiKit Documents"),
                         QStringLiteral("PDF, comic and ebooks viewer controls."),
                         QStringLiteral(Documents_VERSION_STRING),
                         QStringLiteral("http://mauikit.org"),
                         KAboutLicense::LicenseKey::LGPL_V3);
}

KAboutComponent MauiKitDocuments::aboutPoppler()
{
    return KAboutComponent(QStringLiteral("Poppler"),
                         QStringLiteral("PDF rendering library based on the xpdf-3.0 code base."),
                         QStringLiteral(POPPLER_VERSION),
                         QStringLiteral("https://poppler.freedesktop.org/"),
                         KAboutLicense::LicenseKey::LGPL_V3);
}
