# SPDX-FileCopyrightText: 2022 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause


add_subdirectory(code/poppler)
#add_subdirectory(epub)
#add_subdirectory(cbz)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    )


set(
    sources
    code/plugin.cpp
    )


set(
    headers
    code/plugin.h
    )


if(QUICK_COMPILER)
    qtquick_compiler_add_resources(documents_QML_QRC resources.qrc)
else()
    qt5_add_resources(documents_QML_QRC resources.qrc)
endif()

ecm_generate_qmltypes(org.mauikit.documents ${PROJECT_VERSION} DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents)

add_library(
    MauiKitDocuments
    SHARED
    ${sources}
    ${documents_QML_QRC}
    )

add_library(MauiKit::Documents ALIAS MauiKitDocuments)

generate_export_header(MauiKitDocuments BASE_NAME Documents)

set_target_properties(MauiKitDocuments PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${Documents_SOVERSION}
    EXPORT_NAME Documents
    )
target_include_directories(MauiKitDocuments
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MauiKit/Documents>")

target_include_directories(MauiKitDocuments PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR};>")

if(ANDROID)
    target_link_libraries(MauiKitDocuments PRIVATE Qt5::AndroidExtras)
endif()

if(UNIX AND NOT ANDROID)
    target_link_libraries(MauiKitDocuments PRIVATE KF5::ConfigCore)
endif()

if(QUICK_COMPILER)
    add_definitions(-DQUICK_COMPILER)
    target_compile_definitions(MauiKitDocuments PUBLIC QUICK_COMPILER)
endif()

target_link_libraries(
    MauiKitDocuments
    PRIVATE
    PopplerLib
    KF5::I18n
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    MauiKit
    )

install(TARGETS MauiKitDocuments EXPORT MauiKitDocumentsTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    ${headers}
    ${CMAKE_CURRENT_BINARY_DIR}/documents_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/MauiKit/Documents
    COMPONENT Devel)

if (BUILD_SHARED_LIBS)
    add_custom_target(copy_to_bin ALL
        COMMAND ${CMAKE_COMMAND} -E
        make_directory ${CMAKE_BINARY_DIR}/bin/org/mauikit/documents/
        COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/controls ${CMAKE_BINARY_DIR}/bin/org/mauikit/documents/
        COMMAND ${CMAKE_COMMAND} -E
        copy $<TARGET_FILE:MauiKitDocuments> ${CMAKE_BINARY_DIR}/bin/org/mauikit/documents/
        )

    if(QUICK_COMPILER AND NOT ANDROID )
        install(FILES controls/qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents)
    else()
        install(DIRECTORY controls/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents)
    endif()
endif()

install(TARGETS MauiKitDocuments DESTINATION ${KDE_INSTALL_QMLDIR}/org/mauikit/documents )
