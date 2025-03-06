cmake_minimum_required(VERSION 3.16)
project(myproject VERSION 1.0.0 LANGUAGES CXX)

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
    src/main.cpp
    src/MyCanvas.cpp
    include/MyCanvas.h  # Include headers explicitly for better IDE integration
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