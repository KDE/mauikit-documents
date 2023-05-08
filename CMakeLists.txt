# SPDX-FileCopyrightText: 2020 Carl Schwan <carl@carlschwan.eu>
#
# SPDX-License-Identifier: BSD-2-Clause
cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.21)

set(REQUIRED_KF5_VERSION "5.75.0")
set(REQUIRED_QT_VERSION 5.15.0)

set(KF5_MIN_VERSION "5.92.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(POLICY CMP0063)
    cmake_policy (SET CMP0063 NEW)
endif(POLICY CMP0063)

set(AUTOMOC_MOC_OPTIONS -Muri=org.mauikit.documents)

project(Documents LANGUAGES CXX VERSION 1.1.0)

find_package(ECM ${REQUIRED_KF5_VERSION} NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(ECMGenerateQmlTypes)
include(CMakePackageConfigHelpers)
#include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDEPackageAppTemplates)
include(KDEClangFormat)
include(ECMQtDeclareLoggingCategory)

option(QUICK_COMPILER "Use QtQuick compiler to improve performance" TRUE)

if(ANDROID)
    find_package(Qt5 REQUIRED COMPONENTS AndroidExtras)
    find_package(Gradle REQUIRED)
endif()

if(UNIX AND NOT ANDROID)
    find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS Config)
endif()

find_package(Qt5 ${REQUIRED_QT_VERSION} COMPONENTS Core Quick Concurrent REQUIRED)
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS I18n CoreAddons)
find_package(MauiKit)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/MauiKitDocuments")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/MauiKitDocumentsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitDocumentsConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
    )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitDocumentsConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/MauiKitDocumentsConfig.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
    )

install(EXPORT MauiKitDocumentsTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE MauiKitDocumentsTargets.cmake
    NAMESPACE MauiKit::)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX Documents
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/documents_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/MauiKitDocumentsConfigVersion.cmake"
    SOVERSION ${PROJECT_VERSION_MAJOR}
    )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/documents_version.h"
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit/Documents
    COMPONENT Devel)

    if(QUICK_COMPILER)
    find_package(Qt5QuickCompiler)
    set_package_properties(Qt5QuickCompiler PROPERTIES
        DESCRIPTION "Compile QML at build time"
        TYPE OPTIONAL
        )
endif()

add_definitions(-DTRANSLATION_DOMAIN="mauikitdocuments")
ki18n_install(po)

add_subdirectory(src)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
