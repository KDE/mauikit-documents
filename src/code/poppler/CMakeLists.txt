# SPDX-FileCopyrightText: 2021 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause

project(PopplerLib)

if(ANDROID)
    find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS QuickControls2)
elseif(UNIX)
    find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)
    find_package(Poppler REQUIRED COMPONENTS Qt${QT_MAJOR_VERSION})
    set_package_properties("Poppler" PROPERTIES
        TYPE RECOMMENDED
        PURPOSE "Support for PDF files.")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR})

set(sources
    code/pdfdocument.cpp
    code/pdfimageprovider.cpp
    code/thumbnailer.cpp
    code/pdfitem.cpp
    code/pdftocmodel.cpp)

qt_add_resources(pluginData
    data/fonts/fonts.qrc
    img_assets.qrc)

add_library(PopplerLib
    STATIC
    ${sources}
    ${pluginData})

if (ANDROID)
    add_library(poppler STATIC IMPORTED GLOBAL)
    set_target_properties(poppler PROPERTIES IMPORTED_LOCATION "/home/camilo/Qt/6.9.0/android_arm64_v8a/lib/libpoppler_arm64-v8a.so" )

    add_library(poppler-qt6 STATIC IMPORTED GLOBAL)
    set_target_properties(poppler-qt6 PROPERTIES IMPORTED_LOCATION "/home/camilo/Qt/6.9.0/android_arm64_v8a/lib/libpoppler-qt6_arm64-v8a.so" )

    target_include_directories(PopplerLib PUBLIC "/home/camilo/Qt/6.9.0/android_arm64_v8a/include/")

    target_link_libraries(PopplerLib PRIVATE poppler-qt6)
elseif(UNIX)
    target_link_libraries(PopplerLib PRIVATE Poppler::Qt${QT_MAJOR_VERSION})
endif()

set_property(TARGET PopplerLib PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(PopplerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PopplerLib
    PRIVATE
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Concurrent)

