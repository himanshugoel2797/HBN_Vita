## This file is a quick tutorial on writing CMakeLists for targeting the Vita
cmake_minimum_required(VERSION 2.8)

## This includes the Vita toolchain, must go before project definition
# It is a convenience so you do not have to type 
# -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake for cmake. It is 
# highly recommended that you include this block for all projects.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

## Define project parameters here
# Name of the project
project(psvhbn)
# This line adds Vita helper macros, must go after project definition in order 
# to build Vita specific artifacts (self/vpk).
include("${VITASDK}/share/vita.cmake" REQUIRED)

## Configuration options for this app
# Display name (under bubble in LiveArea)
set(VITA_APP_NAME "The Homebrew Network")
# Unique ID must be exactly 9 characters. Recommended: XXXXYYYYY where X = 
# unique string of developer and Y = a unique number for this app
set(VITA_TITLEID  "THBN00001")
# Optional version string to show in LiveArea's more info screen
set(VITA_VERSION  "01.00")

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
# Optional. You can specify more param.sfo flags this way.
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

# Add any additional include paths here
include_directories(
  inc
  libscenegraph/inc
)

# Add any additional library paths here
# ${CMAKE_CURRENT_BINARY_DIR} lets you use any library currently being built
link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

## Build and link
# Add all the files needed to compile here
add_executable(psvhbn
  libscenegraph/src/scenemanager.c
  src/helpers/display.c
  src/helpers/net.c
  src/helpers/json.c
  src/helpers/json_helpers.c
  src/helpers/file.c
  src/helpers/hbn_coms.c
  src/scenes/entry_scene.c
  src/scenes/gekihen_scene.c
  src/scenes/com_init_thread.c
  src/scenes/category_scene.c
  src/ui/ui.c
  src/main.c
)

# Library to link to (drop the -l prefix). This will mostly be stubs.
target_link_libraries(psvhbn
  vita2d
  png16
  jpeg
  freetype
  z
  m
  SceLibKernel_stub
  SceSysmodule_stub
  SceCtrl_stub
  SceTouch_stub
  SceNet_stub
  SceNetCtl_stub
  SceSsl_stub
  SceHttp_stub
  SceAppUtil_stub
  ScePgf_stub
  ScePower_stub
  SceCommonDialog_stub
  SceGxm_stub
  SceDisplay_stub
)

## Create Vita files
vita_create_self(hbn.self psvhbn)
# The FILE directive lets you add additional files to the VPK, the syntax is 
# FILE src_path dst_path_in_vpk. In this case, we add the LiveArea paths.
vita_create_vpk(hbn.vpk ${VITA_TITLEID} hbn.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE data/gekihen_splash.png data/gekihen_splash.png
  FILE data/hbn.png data/hbn.png
)