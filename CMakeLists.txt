cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(gdshared)

add_library(gdshared SHARED
    src/main.cpp
    src/ext/ZlibHelper.cpp
    src/layers/EditLevelLayer.cpp
    src/layers/ExportSettingsLayer.cpp
    src/layers/LevelBrowserLayer.cpp
    src/layers/LevelInfoLayer.cpp
    src/utils/gdshare.cpp
)

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:HJfod/cocos-headers#cbf79fc")
CPMAddPackage("gh:HJfod/gd.h#90f2110")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")
CPMAddPackage("gh:mlabbe/nativefiledialog#2850c97")
CPMAddPackage("gh:sebastiandev/zipper#adf66e8")
CPMAddPackage("gh:hjfod/gd-nodes#38ac99d")

target_compile_definitions(gdshared PRIVATE CC_DLL= FTG_IGNORE_UNICODE=1)

target_include_directories(gdshared PRIVATE
    nuts
    ${gd.h_SOURCE_DIR}/include ${gd.h_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include
    ${zipper_SOURCE_DIR}/zipper
    # suck my nuts zipper
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/zipper-build/zlib_install/include
)

add_library(nfd INTERFACE)

target_sources(nfd INTERFACE
    ${nativefiledialog_SOURCE_DIR}/src/nfd_common.c
    ${nativefiledialog_SOURCE_DIR}/src/nfd_win.cpp
)

target_include_directories(nfd INTERFACE ${nativefiledialog_SOURCE_DIR}/src/include)

add_library(nodes INTERFACE)

target_sources(nodes INTERFACE
    ${gd-nodes_SOURCE_DIR}/BGCheckbox.cpp
    ${gd-nodes_SOURCE_DIR}/BrownAlertDelegate.cpp
    ${gd-nodes_SOURCE_DIR}/HorizontalSelectMenu.cpp
    ${gd-nodes_SOURCE_DIR}/InputNode.cpp
    ${gd-nodes_SOURCE_DIR}/InputPrompt.cpp
    ${gd-nodes_SOURCE_DIR}/MonoSpaceLabel.cpp
    ${gd-nodes_SOURCE_DIR}/PickMenu.cpp
)

target_include_directories(nodes INTERFACE ${gd-nodes_SOURCE_DIR})


target_link_libraries(gdshared mat-dash cocos2d minhook staticZipper nodes nfd)