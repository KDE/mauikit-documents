project(KRar C)


find_package(KF${KF_MAJOR_VERSION} ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS Archive)
find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core )


set(unarr_SRCS
    unarr/rar/uncompress-rar.c
    unarr/rar/huffman-rar.c
    unarr/rar/rar.c
    unarr/rar/filter-rar.c
    unarr/rar/rarvm.c
    unarr/rar/parse-rar.c
    unarr/lzmasdk/Ppmd7.c
    unarr/lzmasdk/Ppmd8.c
    unarr/lzmasdk/CpuArch.c
    unarr/lzmasdk/LzmaDec.c
    unarr/lzmasdk/Ppmd7Dec.c
    unarr/lzmasdk/Ppmd8Dec.c
    unarr/common/custalloc.c
    unarr/common/unarr.c
    unarr/common/stream.c
    unarr/common/conv.c
    unarr/common/crc32.c)

add_library(karchive-c-unarr STATIC ${unarr_SRCS})
set_property(TARGET karchive-c-unarr PROPERTY POSITION_INDEPENDENT_CODE ON)

if (UNIX OR MINGW)
#    target_compile_options(karchive-c-unarr PUBLIC  KARCHIVE-C-UNARR)
    target_compile_options(karchive-c-unarr PRIVATE -std=gnu99 -fomit-frame-pointer -D_FILE_OFFSET_BITS=64 -fPIC)
    set_property(TARGET karchive-c-unarr PROPERTY AUTOMOC OFF)
endif (UNIX OR MINGW)

if (ZLIB_FOUND)
    target_include_directories(karchive-c-unarr PRIVATE ${ZLIB_INCLUDE_DIRS})
endif(ZLIB_FOUND)

add_library(iface_karchive-c-unarr  INTERFACE)
target_link_libraries(iface_karchive-c-unarr  INTERFACE karchive-c-unarr  $<TARGET_OBJECTS:karchive-c-unarr>)

set(karchive_rar_SRCS
    KRar.cpp
    KRarFileEntry.cpp)

add_library (krarlib
    STATIC
    ${karchive_rar_SRCS})

target_include_directories(krarlib
    PUBLIC
    unarr
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (krarlib
    PUBLIC
    iface_karchive-c-unarr

    Qt${QT_MAJOR_VERSION}::Core
    KF${KF_MAJOR_VERSION}::Archive)
