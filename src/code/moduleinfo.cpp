#include "moduleinfo.h"
#include "../documents_version.h"
#include <KI18n/KLocalizedString>

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
                         i18n("PDF, comic and ebooks viewer controls."),
                         QStringLiteral(Documents_VERSION_STRING),
                         QStringLiteral("http://mauikit.org"),
                         KAboutLicense::LicenseKey::LGPL_V3);
}
