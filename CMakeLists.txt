cmake_minimum_required(VERSION 3.5)
project(Fly-simulator)

add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory("Show me image")

add_custom_target(Fly-simulator)
add_dependencies(Fly-simulator Fly-simulator-client Fly-simulator-server Fly-simulator-show-me-image)
