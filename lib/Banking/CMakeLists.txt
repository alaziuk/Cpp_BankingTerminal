set(LIBRARY_NAME BankingLib)

add_library(${LIBRARY_NAME} Banking.cpp)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)