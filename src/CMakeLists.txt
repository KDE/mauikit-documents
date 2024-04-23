# SPDX-FileCopyrightText: 2022 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY)

add_subdirectory(code/poppler)
#add_subdirectory(epub)
add_subdirectory(code/cbz)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code)

if (QT_MAJOR_VERSION STREQUAL "6")
    add_library(MauiKitDocuments${MAUI_MAJOR_VERSION} SHARED)
endif()

ecm_add_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION}
    URI "org.mauikit.documents"
    CLASS_NAME DocumentsPlugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE)

set(headers
    code/moduleinfo.h)

if (QT_MAJOR_VERSION STREQUAL "5")
    if(QUICK_COMPILER)
        qtquick_compiler_add_resources(documents_QML_QRC resources.qrc)
    else()
        qt5_add_resources(documents_QML_QRC resources.qrc)
    endif()
endif()

ecm_target_qml_sources(MauiKitDocuments${MAUI_MAJOR_VERSION} PATH poppler SOURCES
    controls.${QT_MAJOR_VERSION}/poppler/PDFViewer.qml)

if (QT_MAJOR_VERSION STREQUAL "6") ##For Qt6 the plugin is split from the shared library, thus we only link the sources to the plugin that are merely part of the plugin and not form the public shared lib. For Qt5 everything is smashed together
    target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}
        PRIVATE
        code/moduleinfo.cpp)
    
    target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}plugin
        PRIVATE
        code/plugin.cpp
        code/plugin.h
        code/poppler/img_assets.qrc)
else()
    
    target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}
        PRIVATE
        code/plugin.cpp
        code/moduleinfo.cpp
        code/poppler/img_assets.qrc)
    
endif()

add_library(MauiKit${MAUI_MAJOR_VERSION}::Documents ALIAS MauiKitDocuments${MAUI_MAJOR_VERSION})

if (QT_MAJOR_VERSION STREQUAL "5")
    if(QUICK_COMPILER)
        target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION} PRIVATE ${documents_QML_QRC})

        add_definitions(-DQUICK_COMPILER)
        target_compile_definitions(MauiKitDocuments${MAUI_MAJOR_VERSION} PUBLIC QUICK_COMPILER)
    endif()
endif()

generate_export_header(MauiKitDocuments${MAUI_MAJOR_VERSION} BASE_NAME Documents)
set_target_properties(MauiKitDocuments${MAUI_MAJOR_VERSION} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME "Documents")

target_include_directories(MauiKitDocuments${MAUI_MAJOR_VERSION}
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Documents>")

target_include_directories(MauiKitDocuments${MAUI_MAJOR_VERSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

if(ANDROID)
    target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION} PRIVATE Qt${QT_MAJOR_VERSION}::AndroidExtras)
endif()

if(UNIX AND NOT ANDROID)
    target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION} PRIVATE KF${KF_MAJOR_VERSION}::ConfigCore)
endif()

if (QT_MAJOR_VERSION STREQUAL "6")
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
    
else()
    target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION}
        PRIVATE
        PopplerLib
        CBZLib
        KF${KF_MAJOR_VERSION}::I18n
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        MauiKit${MAUI_MAJOR_VERSION})
endif()

ecm_finalize_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR})
# ecm_generate_qmltypes(org.mauikit.documents 3.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents)

install(TARGETS MauiKitDocuments${MAUI_MAJOR_VERSION} EXPORT MauiKitDocuments${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/documents_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Documents
    COMPONENT Devel)
