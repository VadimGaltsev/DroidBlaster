# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -stdlib=libc++")


file(GLOB_RECURSE
            mainData

            src/main/cpp/*.h
            src/main/cpp/*.cpp
            )

add_library( # Sets the name of the library.
             droidblaster

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${mainData} )

add_library( # Sets the name of the path variable.
              native-glue

              STATIC
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              ~/Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.c)


set(png_src src/main/lib/libpng)
set(zlib_src src/main/lib/zlib)
set(box2d_src src/main/lib/Box2D)


message("native source app ${PROJECT_SOURCE_DIR}")

file(GLOB
          png_lib
          ${png_src}/*.h
          ${png_src}/*.cpp
          ${png_src}/*.c
            )
file(GLOB
          zlib_lib
          ${zlib_src}/*.h
          ${zlib_src}/*.cpp
          ${zlib_src}/*.c
            )
file(GLOB_RECURSE
          box2dsrc
          ${box2d_src}/*.h
          ${box2d_src}/*.cpp
            )


include_directories(${png_src})
include_directories(${zlib_src})
include_directories(${box2d_src})


add_library( # Sets the name of the path variable.
              pnglib

              STATIC
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              ${png_lib})
add_library( # Sets the name of the path variable.
              zlibLib

              STATIC
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              ${zlib_lib})

add_library( # Sets the name of the path variable.
              box2dlib

              STATIC
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              ${box2dsrc})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library( # Sets the name of the path variable.
                            android-native

                            # Specifies the name of the NDK library that
                            # you want CMake to locate.
                            android )
find_library( # Sets the name of the path variable.
                            eglLib

                            # Specifies the name of the NDK library that
                            # you want CMake to locate.
                            EGL )


find_library( GLES2Lib

              GLESv2)



set(graphics ${GLES2Lib} ${eglLib})


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       droidblaster

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       ${android-native}
                       native-glue
                       ${graphics}
                       pnglib
                       zlibLib
                       box2dlib
                             )