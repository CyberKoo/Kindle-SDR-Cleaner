cmake_minimum_required(VERSION 3.16)
project(Kindle-SDR-Cleaner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -flto -fno-rtti")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb -gdwarf-3 -Wall")

set(SdrCleaner sdr_cleaner.h sdr_cleaner.cpp)

add_executable(sdr_cleaner main.cpp ${SdrCleaner})
