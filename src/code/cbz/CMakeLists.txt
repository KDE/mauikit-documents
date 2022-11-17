

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    PURPOSE "Required for the unarr based rar support used for reading books in the CBR format"
    TYPE REQUIRED
)

find_package(Qt5 REQUIRED COMPONENTS Sql)
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS Archive KIO GuiAddons IconThemes)

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
    code/TextViewerItem.cpp
)

add_library (
    CBZLib
    STATIC
    ${qmlplugin_SRCS}
   )

target_include_directories(CBZLib
    PUBLIC
    code/acbf
    ${Qt5Quick_PRIVATE_INCLUDE_DIRS}
)

if(UNIX AND NOT ANDROID)
    find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED COMPONENTS FileMetaData)
    target_link_libraries (CBZLib
        PUBLIC
        KF5::FileMetaData
    )

add_definitions(-DKFILEMETADATA_FOUND)
target_compile_definitions(CBZLib PUBLIC KFILEMETADATA_FOUND)

endif()

target_link_libraries (CBZLib
    PUBLIC
    acbf
    krarlib
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    Qt5::Sql
    KF5::Archive
    KF5::I18n
    KF5::ConfigCore
    KF5::KIOWidgets
    KF5::KIOCore
    KF5::IconThemes
    KF5::GuiAddons
)

if (ZLIB_FOUND)
    target_include_directories(CBZLib PRIVATE ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(CBZLib PRIVATE ${ZLIB_LIBRARIES})
    add_definitions(-DHAVE_ZLIB)
endif(ZLIB_FOUND)
