cmake_minimum_required(VERSION 3.16)
project(editor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic handling of Qt MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif()

# Add executable
add_executable(${PROJECT_NAME}
    main.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Widgets
)

# Specify MACOSX_BUNDLE for proper app bundle creation on macOS
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
)

# =================================this is testing code=================================
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_executable(hello_test hello_test.cpp)
# target_include_directories(hello_test PRIVATE ${gtest_SOURCE_DIR}/include
#                                                 ${gmock_SOURCE_DIR}/include)
# target_link_libraries(hello_test gtest_main)
# include (GoogleTest)
# gtest_discover_tests(hello_test)
# =================================end of testing code=================================