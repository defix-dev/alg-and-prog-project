cmake_minimum_required(VERSION 3.10)
project(AppModule)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")

set(LIBPQ_PATH "C:/msys64/mingw64/")
set(LIBPQXX_PATH ${LIBPQ_PATH})
set(ASIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/asio-1.30.2/include")
set(CROW_PATH "${CMAKE_SOURCE_DIR}/lib/Crow")
set(APP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

include_directories(${LIBPQ_PATH}/include)
include_directories(${LIBPQXX_PATH}/include)
include_directories(${ASIO_INCLUDE_DIR})
include_directories(${APP_INCLUDE_DIR})

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
    GIT_TAG 08bcf77a687fb06e34138e9e9fa12a4ecbe12332
)
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "disable building examples" FORCE)
fetchcontent_makeavailable(jwt-cpp)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(AppModule 
src/main.cpp
src/test/Answer.cpp
src/test/Quest.cpp
src/test/Test.cpp
src/app_server/DefaultServerInitializer.cpp
src/app_server/controllers/UserRestController.cpp
src/authentication/AccessRequestValidator.cpp
src/database/postgresql/DataAdapter.cpp
src/database/postgresql/DatabaseInitializer.cpp
src/database/postgresql/DataModificator.cpp
src/database/postgresql/DataRemover.cpp
src/database/postgresql/DataUploader.cpp
src/jwt/JwtAdapter.cpp
src/jwt/JwtRequestParser.cpp
src/jwt/JwtRequestValidator.cpp
src/jwt/JwtValidator.cpp
)

target_include_directories(AppModule PRIVATE ${CROW_PATH}/include)
target_link_libraries(AppModule PRIVATE jwt-cpp::jwt-cpp OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(AppModule PRIVATE cpr::cpr pq pqxx ws2_32 Mswsock)
target_link_libraries(AppModule PRIVATE nlohmann_json::nlohmann_json)
