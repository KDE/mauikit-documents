# SPDX-FileCopyrightText: 2021 Camilo Higuita <milo.h@aol.com>
#
# SPDX-License-Identifier: BSD-2-Clause

project(PopplerLib)

if(ANDROID)
    find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS AndroidExtras QuickControls2)
elseif(UNIX)
    find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)
    find_package(Poppler REQUIRED COMPONENTS Qt5)
    set_package_properties("Poppler" PROPERTIES
        TYPE RECOMMENDED
        PURPOSE "Support for PDF files.")
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
    ${Qt5Qml_PRIVATE_INCLUDE_DIRS}
    )

#add the sources to compile
set(
    sources
    code/pdfdocument.cpp
    code/pdfimageprovider.cpp
    code/thumbnailer.cpp
    code/pdfitem.cpp
#    code/verticalview.cpp
    code/pdftocmodel.cpp
    )

set(
    pluginData
    data/fonts/fonts.qrc
    img_assets.qrc
    )


add_library(
    PopplerLib
    STATIC
    ${sources}
    ${pluginData}
    )

if (ANDROID)
    add_library(poppler STATIC IMPORTED GLOBAL)
    set_target_properties( poppler PROPERTIES IMPORTED_LOCATION "/home/camilo/Qt5152/5.15.2/android/lib/libpoppler_armeabi-v7a.so" )

    add_library(poppler-qt5 STATIC IMPORTED GLOBAL)
    set_target_properties( poppler-qt5 PROPERTIES IMPORTED_LOCATION "/home/camilo/Qt5152/5.15.2/android/lib/libpoppler-qt5_armeabi-v7a.so" )

    target_include_directories(PopplerLib PUBLIC "/home/camilo/Qt5152/5.15.2/android/include/")

    target_link_libraries(PopplerLib PRIVATE poppler-qt5)
elseif(UNIX)
    target_link_libraries(PopplerLib PRIVATE Poppler::Qt5)
endif()

set_property(TARGET PopplerLib PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(PopplerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
    PopplerLib
    PRIVATE
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::Concurrent
    )

