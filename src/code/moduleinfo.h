#pragma once

#include <QString>
#include <KAboutData>
#include "mauikit_export.h"

namespace MauiKitDocuments
{
   MAUIKIT_EXPORT QString versionString();
   MAUIKIT_EXPORT QString buildVersion();
   MAUIKIT_EXPORT KAboutComponent aboutData();

   MAUIKIT_EXPORT KAboutComponent aboutLuv();
};
