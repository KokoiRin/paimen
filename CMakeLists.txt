cmake_minimum_required(VERSION 3.16)
project(PomodoroTimer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置为使用 clang-cl
set(CMAKE_C_COMPILER "clang-cl")
set(CMAKE_CXX_COMPILER "clang-cl")

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5
find_package(Qt5 COMPONENTS Widgets Gui Core REQUIRED)

# 源文件
set(SOURCES
    src/main.cpp
    src/PomodoroTimer.cpp
    src/MainWindow.cpp
    src/MiniWindow.cpp
)

set(HEADERS
    src/PomodoroTimer.h
    src/MainWindow.h
    src/MiniWindow.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# 链接Qt库
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets 
    Qt5::Gui 
    Qt5::Core
)

# Release模式优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE /O2 /Oi /Ot /GL)
    target_link_options(${PROJECT_NAME} PRIVATE /LTCG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
