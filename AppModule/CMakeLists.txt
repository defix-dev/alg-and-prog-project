cmake_minimum_required(VERSION 3.12)
project(MyCppProject)

# Установите стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажите путь к компиляторам
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")

# Путь к библиотекам
set(LIBPQXX_PATH "C:/msys64/mingw64")
set(LIBPQ_PATH "C:/msys64/mingw64")

# Укажите путь к заголовочным файлам
include_directories(${LIBPQXX_PATH}/include)
include_directories(${LIBPQ_PATH}/include)

# Укажите путь к библиотекам
link_directories(${LIBPQXX_PATH}/lib)
link_directories(${LIBPQ_PATH}/lib)

# Добавьте исполняемый файл
add_executable(MyCppProject src/main.cpp)

# Подключите библиотеки
target_link_libraries(MyCppProject pqxx pq)
