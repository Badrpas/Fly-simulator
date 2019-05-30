cmake_minimum_required(VERSION 3.5)

project(Fly-simulator-server)

file(GLOB src
        "*.h"
        "*.cpp"
        )

file(GLOB name_gen_src
        "../Name_Generator_lib/*.h"
        "../Name_Generator_lib/*.cpp"
        )

file(GLOB show_me_img_src
        "../Show me image/Send_const.h"
        )

find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)
include_directories(${SDL_INCLUDE_DIR} ${SDL_NET_INCLUDE_DIRS})

add_executable(Fly-simulator-server ${src} ${name_gen_src} ${show_me_img_src})
target_link_libraries(Fly-simulator-server ${SDL_LIBRARY} ${SDL_NET_LIBRARIES})
