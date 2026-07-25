cmake_minimum_required(VERSION 3.10)
project(terroria VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(terroria ${SOURCES})

file(GLOB_RECURSE ALL_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# 2. Extract the folder path for every header found
set(ALL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
foreach(HEADER_FILE ${ALL_HEADER_FILES})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND ALL_INCLUDE_DIRS ${HEADER_DIR})
endforeach()

# 3. Remove duplicate folders from the list
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

# include dirs
target_include_directories(terroria PRIVATE
    ${ALL_INCLUDE_DIRS}
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/include"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_ttf-2.20.2/include"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_image-2.6.3/include"
)

# link libs
target_link_libraries(terroria PRIVATE
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/lib/x64/SDL2main.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2-2.26.5/lib/x64/SDL2.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_ttf-2.20.2/lib/x64/SDL2_ttf.lib"
    "C:/Users/george/Documents/c++ libraries/SDL/SDL2_image-2.6.3/lib/x64/SDL2_image.lib"
)
