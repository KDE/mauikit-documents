#pragma once

#include <QString>
#include <KAboutData>
#include "documents_export.h"

namespace MauiKitDocuments
{
   DOCUMENTS_EXPORT QString versionString();
   DOCUMENTS_EXPORT QString buildVersion();
   DOCUMENTS_EXPORT KAboutComponent aboutData();

   DOCUMENTS_EXPORT KAboutComponent aboutPoppler();
};
