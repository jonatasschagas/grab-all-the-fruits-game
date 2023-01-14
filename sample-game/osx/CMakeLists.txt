cmake_minimum_required(VERSION 3.13.1)

Message("")
Message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
Message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
Message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
Message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
Message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
Message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
Message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )

project(game)

# ------- Set Vars ------- #
## -> Author: https://github.com/JodyAndrews/SDL2_Basic_Setup/

set(CMAKE_C_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# for debugging (-g flag) https://bytefreaks.net/programming-2/cc-how-do-you-set-gdb-debug-flag-g-with-cmake
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(PROJECT_SOURCE_DIR ../..)

set(CMAKE_CXX_FLAGS --std=c++14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(SOURCE_FILES    main.cpp)
set(IMGUI ${PROJECT_SOURCE_DIR}/third-party/imgui)
set(SDL_GAME_IMPL ${PROJECT_SOURCE_DIR}/sdl)
set(ENGINE_IMPL ${PROJECT_SOURCE_DIR}/engine)
set(GAME_IMPL ${PROJECT_SOURCE_DIR}/sample-game/src)

Message("")
Message( STATUS "GAME_IMPL : " ${GAME_IMPL} )
Message( STATUS "ENGINE_IMPL : " ${ENGINE_IMPL} )
Message( STATUS "SDL_GAME_IMPL : " ${SDL_GAME_IMPL} )
Message("")

# Add -O0 to remove optimizations when using gcc
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

# ------- End Set Vars --- #

# ------- Executable ----- #
FILE(GLOB_RECURSE SRC_FILES ${SDL_GAME_IMPL}/*.cpp ${ENGINE_IMPL}/*.cpp ${GAME_IMPL}/*.cpp main.cpp)

# cherry picking IMGUI files
FILE(GLOB IMGUI_MAIN ${IMGUI}/*.h ${IMGUI}/*.cpp ${IMGUI}/*.h ${IMGUI}/*.cpp) # add main imgui files
FILE(GLOB_RECURSE IMGUI_MISC_CPP ${IMGUI}/misc/cpp/*.*) # add cpp misc files
FILE(GLOB_RECURSE IMGUI_FONTS ${IMGUI}/misc/fonts/*.ttf) # add font misc files
FILE(GLOB_RECURSE IMGUI_FREETYPE ${IMGUI}/misc/freetype/*.h ${IMGUI}/misc/freetype/*.cpp) # add freetype misc files
FILE(GLOB_RECURSE IMGUI_SINGLE_FILE ${IMGUI}/misc/single_file/*.h ${IMGUI}/misc/single_file/*.cpp) # add single_file misc files
LIST(APPEND SRC_FILES ${IMGUI_MAIN}) 
LIST(APPEND SRC_FILES ${IMGUI_MISC_CPP})
LIST(APPEND SRC_FILES ${IMGUI_FONTS})
LIST(APPEND SRC_FILES ${IMGUI_FREETYPE})
LIST(APPEND SRC_FILES ${IMGUI_SINGLE_FILE})
LIST(APPEND SRC_FILES ${IMGUI}/backends/imgui_impl_sdl.h ${IMGUI}/backends/imgui_impl_sdl.cpp) # add SDL backend

ADD_EXECUTABLE(game ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "game.out")
# ------- End Executable - #

# ------- Finds ---------- #

FIND_PACKAGE(SDL2 REQUIRED)
Message("")
Message( STATUS "FINDING SDL2" )
IF (${SDL2_FOUND})
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    Message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
ELSE()
    Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message( FATAL_ERROR "SDL2 NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2_IMAGE REQUIRED)
Message("")
Message( STATUS "FINDING SDL2_IMAGE" )
Message( STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
IF (${SDL2_IMAGE_FOUND})
    Message( STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
    Message( STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2_IMAGE NOT FOUND" )
ENDIF()

FIND_PACKAGE(SDL2Mixer REQUIRED)
Message("")
Message( STATUS "FINDING SDL2Mixer" )
Message( STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND} )
IF (${SDL2Mixer_FOUND})
    Message( STATUS "SDL2Mixer_INCLUDE_DIR: " ${SDL2Mixer_INCLUDE_DIR})
    Message( STATUS "SDL2Mixer_LIBRARY: " ${SDL2Mixer_LIBRARY})
ELSE()
    Message( FATAL_ERROR "SDL2Mixer NOT FOUND" )
ENDIF()

FIND_PACKAGE(GLM REQUIRED)
Message("")
Message( STATUS "FINDING GLM" )
Message( STATUS "GLM_FOUND: " ${GLM_FOUND} )
IF (${GLM_FOUND})
    Message( STATUS "GLM_INCLUDE_DIR: " ${GLM_INCLUDE_DIR})
    Message( STATUS "GLM_LIBRARY: GLM is header only library")
ELSE()
    Message( FATAL_ERROR "GLM NOT FOUND" )
ENDIF()

FIND_PACKAGE(Rapidjson REQUIRED)
Message("")
Message( STATUS "FINDING Rapidjson" )
Message( STATUS "RAPIDJSON_FOUND: " ${RAPIDJSON_FOUND} )
IF (${RAPIDJSON_FOUND})
    Message( STATUS "RAPIDJSON_INCLUDE_DIRS: " ${RAPIDJSON_INCLUDE_DIRS})
    Message( STATUS "RAPIDJSON_LIBRARY: RAPIDJSON is header only library")
ELSE()
    Message( FATAL_ERROR "RAPIDJSON NOT FOUND" )
ENDIF()

FIND_PACKAGE(Freetype REQUIRED)
Message("")
Message( STATUS "FINDING FREETYPE" )
Message( STATUS "FREETYPE_FOUND: " ${FREETYPE_FOUND} )
IF (${FREETYPE_FOUND})
    Message( STATUS "FREETYPE_INCLUDE_DIR_freetype2: " ${FREETYPE_INCLUDE_DIR_freetype2})
    Message( STATUS "FREETYPE_INCLUDE_DIR_ft2build: " ${FREETYPE_INCLUDE_DIR_ft2build})
    Message( STATUS "FREETYPE_LIBRARY: " ${FREETYPE_LIBRARY})
ELSE()
    Message( FATAL_ERROR "FREETYPE NOT FOUND" )
ENDIF()

Message("")


# ------- End Finds ------ #

# ------- Inc & Link ---- #

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${RAPIDJSON_INCLUDE_DIRS} ${THIRD_PARTY_LIBS} ${ENGINE_IMPL} ${SDL_GAME_IMPL} ${GAME_IMPL} ${IMGUI} ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY} ${FREETYPE_LIBRARY})
# ------- End ----------- #