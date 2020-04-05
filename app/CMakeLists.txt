cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/cpp/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}")


file(GLOB src_files src/main/cpp/*.cpp)

add_library( # Sets the name of the library.
        native-lib
        SHARED
        ${src_files})

target_link_libraries( # Specifies the target library.
        native-lib
        -Wl,--start-group
        avformat avcodec avfilter avutil swresample swscale
        -Wl,--end-group
        log
        z
        android
        OpenSLES)