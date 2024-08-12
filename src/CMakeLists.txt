# SPDX-FileCopyrightText: 2022 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY)

add_subdirectory(code/poppler)
#add_subdirectory(epub)
add_subdirectory(code/cbz)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code)

    add_library(MauiKitDocuments${MAUI_MAJOR_VERSION} SHARED)

ecm_add_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION}
    URI "org.mauikit.documents"
    CLASS_NAME DocumentsPlugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE)

set(headers
    code/moduleinfo.h)

qt_add_resources(documents_QML_QRC code/poppler/img_assets.qrc)

ecm_target_qml_sources(MauiKitDocuments${MAUI_MAJOR_VERSION} PATH poppler SOURCES
    controls.${QT_MAJOR_VERSION}/poppler/PDFViewer.qml)

    target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}
        PRIVATE
        code/moduleinfo.cpp)

    target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}plugin
        PRIVATE
        code/plugin.cpp
        code/plugin.h
        ${documents_QML_QRC})


add_library(MauiKit${MAUI_MAJOR_VERSION}::Documents ALIAS MauiKitDocuments${MAUI_MAJOR_VERSION})

generate_export_header(MauiKitDocuments${MAUI_MAJOR_VERSION} BASE_NAME Documents)
set_target_properties(MauiKitDocuments${MAUI_MAJOR_VERSION} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME "Documents")

target_include_directories(MauiKitDocuments${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Documents>")

target_include_directories(MauiKitDocuments${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

if(UNIX AND NOT ANDROID)
    target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION} PRIVATE KF${KF_MAJOR_VERSION}::ConfigCore)
endif()

target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION}plugin
    PRIVATE
    PopplerLib
    CBZLib
    KF${KF_MAJOR_VERSION}::I18n
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    MauiKit${MAUI_MAJOR_VERSION})

target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION}
    PRIVATE
    PopplerLib
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::CoreAddons
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml)

ecm_finalize_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR})

install(TARGETS MauiKitDocuments${MAUI_MAJOR_VERSION} EXPORT MauiKitDocuments${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/documents_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Documents
    COMPONENT Devel)
