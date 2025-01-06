cmake_minimum_required(VERSION 3.14)
project(AppModule)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBPQ_PATH "C:/msys64/mingw64/")
set(LIBPQXX_PATH ${LIBPQ_PATH})
set(ASIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/asio-1.30.2/include")
set(CROW_PATH "${CMAKE_SOURCE_DIR}/lib/Crow")

include_directories(${LIBPQ_PATH}/include)
include_directories(${LIBPQXX_PATH}/include)
include_directories(${ASIO_INCLUDE_DIR})

link_directories(${LIBPQ_PATH}/lib)
link_directories(${LIBPQXX_PATH}/lib)

add_subdirectory(${CROW_PATH})

include(FetchContent)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG dec9422db3af470641f8b0d90e4b451c4daebf64
)
FetchContent_MakeAvailable(cpr)

add_executable(AppModule src/main.cpp)

target_include_directories(AppModule PRIVATE ${CROW_PATH}/include)
target_link_libraries(AppModule PRIVATE cpr::cpr pq pqxx ws2_32 Mswsock)
