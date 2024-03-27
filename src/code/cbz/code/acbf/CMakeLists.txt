set(acbf_SRCS
    AcbfAuthor.cpp
    AcbfBinary.cpp
    AcbfBody.cpp
    AcbfBookinfo.cpp
    AcbfContentrating.cpp
    AcbfData.cpp
    AcbfDatabaseref.cpp
    AcbfDocument.cpp
    AcbfDocumentinfo.cpp
    AcbfFrame.cpp
    AcbfIdentifiedObjectModel.cpp
    AcbfInternalReference.cpp
    AcbfInternalReferenceObject.cpp
    AcbfJump.cpp
    AcbfLanguage.cpp
    AcbfMetadata.cpp
    AcbfPage.cpp
    AcbfPublishinfo.cpp
    AcbfReferences.cpp
    AcbfReference.cpp
    AcbfSequence.cpp
    AcbfStyle.cpp
    AcbfStyleSheet.cpp
    AcbfTextarea.cpp
    AcbfTextlayer.cpp)

set(acbf_HEADERS
    AcbfAuthor.h
    AcbfBinary.h
    AcbfBody.h
    AcbfBookinfo.h
    AcbfContentrating.h
    AcbfData.h
    AcbfDatabaseref.h
    AcbfDocument.h
    AcbfDocumentinfo.h
    AcbfFrame.h
    AcbfIdentifiedObjectModel.h
    AcbfInternalReference.h
    AcbfInternalReferenceObject.h
    AcbfJump.h
    AcbfLanguage.h
    AcbfMetadata.h
    AcbfPage.h
    AcbfPublishinfo.h
    AcbfReferences.h
    AcbfReference.h
    AcbfSequence.h
    AcbfStyle.h
    AcbfStyleSheet.h
    AcbfTextarea.h
    AcbfTextlayer.h)

ecm_qt_declare_logging_category(acbf_SRCS
    HEADER acbf_debug.h
    IDENTIFIER ACBF_LOG
    CATEGORY_NAME org.kde.peruse.acbf
    DEFAULT_SEVERITY Warning
    DESCRIPTION "Advanced Comic Book Format (aka ACBF)"
    EXPORT AdvancedComicBookFormat)

add_library(acbf STATIC ${acbf_SRCS})
set_property(TARGET acbf PROPERTY POSITION_INDEPENDENT_CODE ON)

include(ECMGenerateExportHeader)

target_link_libraries(acbf
    PUBLIC
    Qt${QT_MAJOR_VERSION}::Core)
#target_include_directories(acbf
#    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/AdvancedComicBookFormat>"
#)

