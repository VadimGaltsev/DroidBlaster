# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB
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
              /Users/vadik/Library/Android/sdk/ndk-bundle/sources/android/native_app_glue/android_native_app_glue.c)

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
                       )