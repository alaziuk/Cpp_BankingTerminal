cmake_minimum_required(VERSION 3.22.1)

project(BankingApp VERSION 1.0)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

add_subdirectory(lib)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC BankingLib)