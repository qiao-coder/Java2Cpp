# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("java2cpp")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB_RECURSE UTIL_SRC util/*.cpp)
file(GLOB_RECURSE MEDIA_SRC media/*.cpp)
aux_source_directory(. DIR_SRC)

add_library( # Sets the name of the library.
        java2cpp

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        ${UTIL_SRC})

add_library(
        mediaplayer
        SHARED
        android_media_MediaPlayer.cpp
        ${UTIL_SRC}
        ${MEDIA_SRC}
)

include_directories(
        include
#        include/media
#        include/util
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
set(CMAKE_CXX_FLAGS " -L${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${CMAKE_ANDROID_ARCH_ABI}")
target_link_libraries( # Specifies the target library.
        java2cpp

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        a
        b)

target_link_libraries(
        mediaplayer
        ${log-lib})