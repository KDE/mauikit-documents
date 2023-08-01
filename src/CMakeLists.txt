# SPDX-FileCopyrightText: 2022 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause

remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY)

add_subdirectory(code/poppler)
#add_subdirectory(epub)
add_subdirectory(code/cbz)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code)

ecm_add_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION} URI "org.mauikit.documents" VERSION 3.0 CLASSNAME DocumentsPlugin)

set(sources
    code/plugin.cpp)

set(headers
    code/plugin.h)

if (QT_MAJOR_VERSION STREQUAL "5")
    if(QUICK_COMPILER)
        qtquick_compiler_add_resources(documents_QML_QRC resources.qrc)
    else()
        qt5_add_resources(documents_QML_QRC resources.qrc)
    endif()
endif()

ecm_target_qml_sources(MauiKitDocuments${MAUI_MAJOR_VERSION} PATH poppler SOURCES
    controls.${QT_MAJOR_VERSION}/poppler/PDFViewer.qml)

target_sources(MauiKitDocuments${MAUI_MAJOR_VERSION}
    PRIVATE
    ${sources})

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

target_link_libraries(MauiKitDocuments${MAUI_MAJOR_VERSION}
    PRIVATE
    PopplerLib
    CBZLib
    KF${KF_MAJOR_VERSION}::I18n
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    MauiKit${MAUI_MAJOR_VERSION})

ecm_finalize_qml_module(MauiKitDocuments${MAUI_MAJOR_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR})
ecm_generate_qmltypes(org.mauikit.documents 3.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents)

install(TARGETS MauiKitDocuments${MAUI_MAJOR_VERSION} EXPORT MauiKitDocuments${MAUI_MAJOR_VERSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/documents_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit${MAUI_MAJOR_VERSION}/Documents
    COMPONENT Devel)
