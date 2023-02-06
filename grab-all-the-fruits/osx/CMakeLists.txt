cmake_minimum_required(VERSION 3.13.1)

message("")
message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )

project(game) # name of executable

# ------- set Vars ------- #
## -> Author: https://github.com/JodyAndrews/SDL2_Basic_setup/

set(CMAKE_C_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS ON)

# for debugging (-g flag) https://bytefreaks.net/programming-2/cc-how-do-you-set-gdb-debug-flag-g-with-cmake
set(CMAKE_BUILD_TYPE Debug)
set(PROJECT_SOURCE_DIR ../..)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(SOURCE_fileS    main.cpp)
set(IMGUI ${PROJECT_SOURCE_DIR}/third-party/imgui)
set(IMGUI_SDL ${PROJECT_SOURCE_DIR}/third-party/imgui_sdl)
set(THIRD_PARTY_LIBS ${PROJECT_SOURCE_DIR}/third-party/)
set(SDL_GAME_IMPL ${PROJECT_SOURCE_DIR}/sdl)
set(ENGINE_IMPL ${PROJECT_SOURCE_DIR}/engine)
set(GAME_IMPL ${PROJECT_SOURCE_DIR}/grab-all-the-fruits/src)
set(GAME_ASSETS ${PROJECT_SOURCE_DIR}/grab-all-the-fruits/assets)

message("")
message( STATUS "GAME_IMPL : " ${GAME_IMPL} )
message( STATUS "ENGINE_IMPL : " ${ENGINE_IMPL} )
message( STATUS "SDL_GAME_IMPL : " ${SDL_GAME_IMPL} )
message("")

# Add -O0 to remove optimizations when using gcc
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 --std=c++14")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# Adding Support to IMGUI
add_definitions(-DIMGUI_ENABLED)

# ------- End set Vars --- #

# ------- Executable ----- #
file(GLOB_RECURSE SRC_fileS ${SDL_GAME_IMPL}/*.cpp ${ENGINE_IMPL}/*.cpp ${GAME_IMPL}/*.cpp main.cpp)

# cherry picking IMGUI files
file(GLOB IMGUI_MAIN ${IMGUI}/*.h ${IMGUI}/*.cpp ${IMGUI}/*.h ${IMGUI}/*.cpp) # add main imgui files
file(GLOB_RECURSE IMGUI_MISC_CPP ${IMGUI}/misc/cpp/*.*) # add cpp misc files
file(GLOB_RECURSE IMGUI_FONTS ${IMGUI}/misc/fonts/*.ttf) # add font misc files
file(GLOB_RECURSE IMGUI_FREETYPE ${IMGUI}/misc/freetype/*.h ${IMGUI}/misc/freetype/*.cpp) # add freetype misc files
file(GLOB_RECURSE IMGUI_SINGLE_file ${IMGUI}/misc/single_file/*.h ${IMGUI}/misc/single_file/*.cpp) # add single_file misc files
list(APPEND SRC_fileS ${IMGUI_MAIN}) 
list(APPEND SRC_fileS ${IMGUI_MISC_CPP})
list(APPEND SRC_fileS ${IMGUI_FONTS})
list(APPEND SRC_fileS ${IMGUI_FREETYPE})
list(APPEND SRC_fileS ${IMGUI_SINGLE_file})
list(APPEND SRC_fileS ${IMGUI}/backends/imgui_impl_sdl.h ${IMGUI}/backends/imgui_impl_sdl.cpp) # add SDL backend
list(APPEND SRC_fileS ${IMGUI_SDL}/imgui_sdl.h ${IMGUI_SDL}/imgui_sdl.cpp) # add imguil_SDL

add_executable(game ${SRC_fileS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "grab-all-the-fruits")
# ------- End Executable - #

# ------- Finds ---------- #

find_package(SDL2 REQUIRED)
message("")
message( STATUS "FINDING SDL2" )
if (${SDL2_FOUND})
    message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
else()
    message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    message( FATAL_ERROR "SDL2 NOT FOUND" )
endif()

find_package(SDL2_IMAGE REQUIRED)
message("")
message( STATUS "FINDING SDL2_IMAGE" )
message( STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
if (${SDL2_IMAGE_FOUND})
    message( STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
    message( STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
else()
    message( FATAL_ERROR "SDL2_IMAGE NOT FOUND" )
endif()

find_package(SDL2Mixer REQUIRED)
message("")
message( STATUS "FINDING SDL2Mixer" )
message( STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND} )
if (${SDL2Mixer_FOUND})
    message( STATUS "SDL2Mixer_INCLUDE_DIR: " ${SDL2Mixer_INCLUDE_DIR})
    message( STATUS "SDL2Mixer_LIBRARY: " ${SDL2Mixer_LIBRARY})
else()
    message( FATAL_ERROR "SDL2Mixer NOT FOUND" )
endif()

find_package(GLM REQUIRED)
message("")
message( STATUS "FINDING GLM" )
message( STATUS "GLM_FOUND: " ${GLM_FOUND} )
if (${GLM_FOUND})
    message( STATUS "GLM_INCLUDE_DIR: " ${GLM_INCLUDE_DIR})
    message( STATUS "GLM_LIBRARY: GLM is header only library")
else()
    message( FATAL_ERROR "GLM NOT FOUND" )
endif()

find_package(Rapidjson REQUIRED)
message("")
message( STATUS "FINDING Rapidjson" )
message( STATUS "RAPIDJSON_FOUND: " ${RAPIDJSON_FOUND} )
if (${RAPIDJSON_FOUND})
    message( STATUS "RAPIDJSON_INCLUDE_DIRS: " ${RAPIDJSON_INCLUDE_DIRS})
    message( STATUS "RAPIDJSON_LIBRARY: RAPIDJSON is header only library")
else()
    message( FATAL_ERROR "RAPIDJSON NOT FOUND" )
endif()

find_package(Freetype REQUIRED)
message("")
message( STATUS "FINDING FREETYPE" )
message( STATUS "FREETYPE_FOUND: " ${FREETYPE_FOUND} )
if (${FREETYPE_FOUND})
    message( STATUS "FREETYPE_INCLUDE_DIR_freetype2: " ${FREETYPE_INCLUDE_DIR_freetype2})
    message( STATUS "FREETYPE_INCLUDE_DIR_ft2build: " ${FREETYPE_INCLUDE_DIR_ft2build})
    message( STATUS "FREETYPE_LIBRARY: " ${FREETYPE_LIBRARY})
else()
    message( FATAL_ERROR "FREETYPE NOT FOUND" )
endif()

find_package(box2d REQUIRED)
message("")
message( STATUS "FINDING Box2d" )
message( STATUS "box2d_FOUND: " ${box2d_FOUND} )
if (${box2d_FOUND})
    message( STATUS "box2d_INCLUDE_DIR: " ${box2d_INCLUDE_DIR})
    message( STATUS "box2d_LIBRARY: " ${box2d_LIBRARY})
else()
    message( FATAL_ERROR "Box2D NOT FOUND" )
endif()

message("")

# copy assets to build directory
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(game copy_assets)

# ------- End Finds ------ #

# ------- Inc & Link ---- #

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS} ${ENGINE_IMPL} ${SDL_GAME_IMPL} ${GAME_IMPL} ${IMGUI} ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build} ${box2d_INCLUDE_DIR} ${THIRD_PARTY_LIBS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY} ${FREETYPE_LIBRARY} ${box2d_LIBRARY})
# ------- End ----------- #