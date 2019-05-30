cmake_minimum_required(VERSION 3.5)

project(Fly-simulator-show-me-image)

file(GLOB src
        "*.h"
        "*.cpp"
        )

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_net REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIRS} ${SDL_NET_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

add_executable(Fly-simulator-show-me-image ${src})
target_link_libraries(Fly-simulator-show-me-image ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${SDL_NET_LIBRARIES} ${OPENGL_LIBRARY})
