

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    PURPOSE "Required for the unarr based rar support used for reading books in the CBR format"
    TYPE REQUIRED)

find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Sql)
find_package(KF${KF_MAJOR_VERSION} ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS
    Archive
    KIO
    GuiAddons
    IconThemes)

kde_enable_exceptions()

add_subdirectory(code/acbf)
add_subdirectory(code/karchive-rar)

set(qmlplugin_SRCS
    code/ArchiveBookModel.cpp
    code/ArchiveImageProvider.cpp
    code/BookDatabase.cpp
    code/BookModel.cpp
    code/BookListModel.cpp
    code/CategoryEntriesModel.cpp
    code/ComicCoverImageProvider.cpp
    code/FilterProxy.cpp
    code/FolderBookModel.cpp
    code/PreviewImageProvider.cpp
    code/PropertyContainer.cpp
    code/TextDocumentEditor.cpp
    code/TextViewerItem.cpp)

add_library (CBZLib STATIC)
target_sources(CBZLib PRIVATE ${qmlplugin_SRCS})
set_property(TARGET CBZLib PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(CBZLib
    PUBLIC
    code/acbf
    ${Qt${QT_MAJOR_VERSION}Quick_PRIVATE_INCLUDE_DIRS})

if(UNIX AND NOT ANDROID)
    find_package(KF${KF_MAJOR_VERSION} ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS FileMetaData)
    target_link_libraries (CBZLib
        PUBLIC
        KF${KF_MAJOR_VERSION}::FileMetaData)

add_definitions(-DKFILEMETADATA_FOUND)
target_compile_definitions(CBZLib PUBLIC KFILEMETADATA_FOUND)

endif()

target_link_libraries (CBZLib
    PUBLIC
    acbf
    krarlib
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::QuickPrivate
    Qt${QT_MAJOR_VERSION}::Sql
    KF${KF_MAJOR_VERSION}::Archive
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::ConfigCore
    # KF${KF_MAJOR_VERSION}::KIOWidgets
    KF${KF_MAJOR_VERSION}::KIOCore
    KF${KF_MAJOR_VERSION}::IconThemes
    KF${KF_MAJOR_VERSION}::GuiAddons)

if (ZLIB_FOUND)
    target_include_directories(CBZLib PRIVATE ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(CBZLib PRIVATE ${ZLIB_LIBRARIES})
    add_definitions(-DHAVE_ZLIB)
endif(ZLIB_FOUND)
