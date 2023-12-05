cmake_minimum_required(VERSION 3.27)
project(SleepKiller)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(EXECUTABLE_OUTPUT_PATH "../")

add_executable(SleepKiller
        main.cpp
        src/ProcessUtils.cpp
        src/PatternScanUtil.cpp
        src/FunctionUtils.cpp)
