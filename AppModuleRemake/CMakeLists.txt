cmake_minimum_required(VERSION 3.14)
project(AppModule)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")

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

find_package(OpenSSL REQUIRED)

fetchcontent_declare(jwt-cpp 
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG 08bcf77a687fb06e34138e9e9fa12a4ecbe12332 # v0.7.0 release
)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
fetchcontent_makeavailable(jwt-cpp)

add_executable(AppModule src/main.cpp)

target_include_directories(AppModule PRIVATE ${CROW_PATH}/include)
target_link_libraries(AppModule PRIVATE jwt-cpp::jwt-cpp OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(AppModule PRIVATE cpr::cpr pq pqxx ws2_32 Mswsock)
